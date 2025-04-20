#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Test counter variables
int total_tests = 0;
int passed_tests = 0;
int failed_tests = 0;

// Callback tracking
int callback_called = 0;

// Test callback function
void TestCallback(void* userData) {
    int* value = (int*)userData;
    (*value)++;
    callback_called++;
    printf("Callback triggered: %d\n", *value);
}

// Test macros
#define TEST_START(name) do { \
    printf("\n--- TEST: %s ---\n", name); \
    total_tests++; \
} while(0)

#define TEST_ASSERT(condition, message) do { \
    if (!(condition)) { \
        printf("❌ FAILED: %s\n", message); \
        failed_tests++; \
        return; \
    } \
} while(0)

#define TEST_END(name) do { \
    printf("✅ PASSED: %s\n", name); \
    passed_tests++; \
} while(0)

// Test component creation
void TestComponentCreation() {
    TEST_START("Component Creation");
    
    // Test button creation
    RayDialComponent* button = CreateButton(
        (Rectangle){100, 100, 200, 50},
        "Test Button",
        TestCallback,
        NULL
    );
    TEST_ASSERT(button != NULL, "Button creation failed");
    TEST_ASSERT(button->type == RAYDIAL_BUTTON, "Button type incorrect");
    TEST_ASSERT(button->bounds.x == 100, "Button position incorrect");
    TEST_ASSERT(strcmp(((RayDialButtonData*)button->data)->text, "Test Button") == 0, "Button text incorrect");
    
    // Test label creation
    RayDialComponent* label = CreateLabel(
        (Rectangle){100, 200, 300, 50},
        "Test Label",
        true
    );
    TEST_ASSERT(label != NULL, "Label creation failed");
    TEST_ASSERT(label->type == RAYDIAL_LABEL, "Label type incorrect");
    TEST_ASSERT(((RayDialLabelData*)label->data)->wrapText == true, "Label wrap setting incorrect");
    
    // Test panel creation
    RayDialComponent* panel = CreatePanel(
        (Rectangle){50, 50, 400, 300},
        RAYWHITE
    );
    TEST_ASSERT(panel != NULL, "Panel creation failed");
    TEST_ASSERT(panel->type == RAYDIAL_PANEL, "Panel type incorrect");
    TEST_ASSERT(panel->bounds.width == 400, "Panel dimensions incorrect");
    
    // Cleanup
    FreeComponent(button);
    FreeComponent(label);
    FreeComponent(panel);
    
    TEST_END("Component Creation");
}

// Test component hierarchy
void TestComponentHierarchy() {
    TEST_START("Component Hierarchy");
    
    // Create components
    RayDialComponent* panel = CreatePanel((Rectangle){50, 50, 400, 300}, RAYWHITE);
    RayDialComponent* label = CreateLabel((Rectangle){60, 60, 300, 50}, "Test Label", false);
    RayDialComponent* button = CreateButton((Rectangle){60, 120, 200, 50}, "Test Button", TestCallback, NULL);
    
    // Test adding components
    AddComponent(panel, label);
    TEST_ASSERT(panel->next == label, "AddComponent failed to add label");
    TEST_ASSERT(panel->next->next == NULL, "Label next pointer should be NULL");
    
    AddComponent(panel, button);
    TEST_ASSERT(panel->next == label, "Panel's next should still be label");
    TEST_ASSERT(label->next == button, "Label's next should be button");
    TEST_ASSERT(button->next == NULL, "Button's next should be NULL");
    
    // Test removing components
    RemoveComponent(panel, label);
    TEST_ASSERT(panel->next == button, "RemoveComponent failed to remove label");
    TEST_ASSERT(label->next == NULL, "Label's next should be NULL after removal");
    
    // Cleanup
    FreeComponent(panel);
    FreeComponent(label);
    
    TEST_END("Component Hierarchy");
}

// Test dialogue node creation
void TestDialogueNodeCreation() {
    TEST_START("Dialogue Node Creation");
    
    // Create nodes
    RayDialNode* rootNode = CreateDialogueNode("root", "Root Node");
    RayDialNode* childNode = CreateDialogueNode("child", "Child Node");
    
    TEST_ASSERT(rootNode != NULL, "Root node creation failed");
    TEST_ASSERT(childNode != NULL, "Child node creation failed");
    TEST_ASSERT(strcmp(rootNode->id, "root") == 0, "Node ID incorrect");
    TEST_ASSERT(strcmp(rootNode->text, "Root Node") == 0, "Node text incorrect");
    TEST_ASSERT(rootNode->choices == NULL, "New node should not have choices");
    TEST_ASSERT(rootNode->choiceCount == 0, "New node choice count should be 0");
    
    // Test adding choices
    AddChoice(rootNode, childNode);
    TEST_ASSERT(rootNode->choiceCount == 1, "Choice count should be 1 after adding a choice");
    TEST_ASSERT(rootNode->choices[0] == childNode, "Choice reference incorrect");
    
    // Test node callbacks
    int userData = 0;
    SetNodeCallbacks(rootNode, TestCallback, TestCallback, &userData);
    TEST_ASSERT(rootNode->onEnter != NULL, "onEnter callback not set");
    TEST_ASSERT(rootNode->onExit != NULL, "onExit callback not set");
    TEST_ASSERT(rootNode->userData == &userData, "Node user data not set correctly");
    
    // Call the callbacks to test
    callback_called = 0;
    rootNode->onEnter(rootNode->userData);
    TEST_ASSERT(callback_called == 1, "onEnter callback not triggered");
    rootNode->onExit(rootNode->userData);
    TEST_ASSERT(callback_called == 2, "onExit callback not triggered");
    
    // Test cleanup
    free(rootNode->choices);
    free(rootNode);
    free(childNode);
    
    TEST_END("Dialogue Node Creation");
}

// Test dialogue manager creation
void TestDialogueManager() {
    TEST_START("Dialogue Manager");
    
    // Create node and manager
    RayDialNode* rootNode = CreateDialogueNode("root", "Root Node");
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    TEST_ASSERT(manager != NULL, "Manager creation failed");
    TEST_ASSERT(manager->rootNode == rootNode, "Manager root node incorrect");
    TEST_ASSERT(manager->currentNode == rootNode, "Manager current node incorrect");
    TEST_ASSERT(manager->isActive == true, "Manager should be active by default");
    
    // Test node transition
    RayDialNode* childNode = CreateDialogueNode("child", "Child Node");
    AddChoice(rootNode, childNode);
    
    TransitionToNode(manager, "child");
    TEST_ASSERT(manager->currentNode == childNode, "TransitionToNode failed");
    
    TransitionToNode(manager, "root");
    TEST_ASSERT(manager->currentNode == rootNode, "TransitionToNode back to root failed");
    
    // Test cleanup
    // Note: In a real test, we'd use FreeDialogueManager instead
    free(rootNode->choices);
    free(rootNode);
    free(childNode);
    free(manager);
    
    TEST_END("Dialogue Manager");
}

// Test component properties and interaction
void TestComponentProperties() {
    TEST_START("Component Properties");
    
    // Create component
    RayDialComponent* button = CreateButton(
        (Rectangle){100, 100, 200, 50},
        "Test Button",
        TestCallback,
        NULL
    );
    
    // Test visible/enabled
    TEST_ASSERT(button->visible == true, "Button should be visible by default");
    TEST_ASSERT(button->enabled == true, "Button should be enabled by default");
    
    SetComponentVisible(button, false);
    TEST_ASSERT(button->visible == false, "SetComponentVisible failed");
    
    SetComponentEnabled(button, false);
    TEST_ASSERT(button->enabled == false, "SetComponentEnabled failed");
    
    // Test hover behavior
    button->enabled = true;
    
    // We can't test mouse position in an automated test, so we'll just check
    // that the function exists and returns the expected value when not hovered
    bool isHovered = IsComponentHovered(button);
    TEST_ASSERT(isHovered == false, "IsComponentHovered should return false when not hovered");
    
    // Same for click behavior
    bool isClicked = IsComponentClicked(button);
    TEST_ASSERT(isClicked == false, "IsComponentClicked should return false when not clicked");
    
    // Cleanup
    FreeComponent(button);
    
    TEST_END("Component Properties");
}

// Run all tests
void RunAllTests() {
    printf("\n====== RAYDIAL TEST SUITE ======\n");
    
    // Run tests
    TestComponentCreation();
    TestComponentHierarchy();
    TestDialogueNodeCreation();
    TestDialogueManager();
    TestComponentProperties();
    
    // Print summary
    printf("\n====== TEST SUMMARY ======\n");
    printf("Total tests:  %d\n", total_tests);
    printf("Passed tests: %d\n", passed_tests);
    printf("Failed tests: %d\n", failed_tests);
    printf("\n");
    
    if (failed_tests == 0) {
        printf("✅ ALL TESTS PASSED!\n");
    } else {
        printf("❌ SOME TESTS FAILED!\n");
    }
}

int main() {
    // Initialize raylib (minimal window for testing)
    const int screenWidth = 640;
    const int screenHeight = 480;
    InitWindow(screenWidth, screenHeight, "Raydial Test Suite");
    SetTargetFPS(60);
    
    // Run the tests immediately
    RunAllTests();
    
    // Wait for user to close window
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            if (failed_tests == 0) {
                DrawText("✅ ALL TESTS PASSED!", 190, 200, 20, GREEN);
            } else {
                DrawText(TextFormat("❌ %d TESTS FAILED!", failed_tests), 190, 200, 20, RED);
            }
            
            DrawText("Press ESC to exit", 250, 240, 20, DARKGRAY);
        EndDrawing();
    }
    
    // Close window
    CloseWindow();
    
    return 0;
} 