#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <stdio.h>
#include <cmocka.h>

#include "raylib.h"
#include "raydial.h"

// Test fixture data
typedef struct {
    int callbackCounter;
    int userData;
    RayDialNode* rootNode;
    RayDialNode* childNode;
    RayDialManager* manager;
} TestState;

// Mock callback function for button testing
static void test_callback(void* userData) {
    TestState* state = (TestState*)userData;
    state->callbackCounter++;
    state->userData++;
}

// Test fixtures: setup and teardown

static int setup(void **state) {
    TestState* testState = malloc(sizeof(TestState));
    memset(testState, 0, sizeof(TestState));
    *state = testState;
    return 0;
}

static int teardown(void **state) {
    free(*state);
    return 0;
}

// Test cases

// Component creation tests
static void test_button_creation(void **state) {
    // Create a button component
    RayDialComponent* button = CreateButton(
        (Rectangle){100, 100, 200, 50},
        "Test Button",
        test_callback,
        *state
    );
    
    // Verify button was created correctly
    assert_non_null(button);
    assert_int_equal(button->type, RAYDIAL_BUTTON);
    assert_int_equal(button->bounds.x, 100);
    assert_int_equal(button->bounds.y, 100);
    assert_int_equal(button->bounds.width, 200);
    assert_int_equal(button->bounds.height, 50);
    assert_string_equal(((RayDialButtonData*)button->data)->text, "Test Button");
    assert_true(button->enabled);
    assert_true(button->visible);
    assert_non_null(button->onClick);
    assert_ptr_equal(button->userData, *state);
    
    // Cleanup
    FreeComponent(button);
}

static void test_label_creation(void **state) {
    // Create a label component
    RayDialComponent* label = CreateLabel(
        (Rectangle){100, 200, 300, 50},
        "Test Label",
        true
    );
    
    // Verify label was created correctly
    assert_non_null(label);
    assert_int_equal(label->type, RAYDIAL_LABEL);
    assert_int_equal(label->bounds.x, 100);
    assert_int_equal(label->bounds.y, 200);
    assert_int_equal(label->bounds.width, 300);
    assert_true(((RayDialLabelData*)label->data)->wrapText);
    assert_string_equal(((RayDialLabelData*)label->data)->text, "Test Label");
    
    // Cleanup
    FreeComponent(label);
}

static void test_panel_creation(void **state) {
    // Create a panel component
    RayDialComponent* panel = CreatePanel(
        (Rectangle){50, 50, 400, 300},
        RAYWHITE
    );
    
    // Verify panel was created correctly
    assert_non_null(panel);
    assert_int_equal(panel->type, RAYDIAL_PANEL);
    assert_int_equal(panel->bounds.width, 400);
    assert_int_equal(panel->bounds.height, 300);
    
    // Cleanup
    FreeComponent(panel);
}

// Component hierarchy tests
static void test_component_hierarchy(void **state) {
    // Create components
    RayDialComponent* panel = CreatePanel((Rectangle){50, 50, 400, 300}, RAYWHITE);
    RayDialComponent* label = CreateLabel((Rectangle){60, 60, 300, 50}, "Test Label", false);
    RayDialComponent* button = CreateButton((Rectangle){60, 120, 200, 50}, "Test Button", test_callback, *state);
    
    // Test adding components
    AddComponent(panel, label);
    assert_ptr_equal(panel->next, label);
    assert_null(label->next);
    
    AddComponent(panel, button);
    assert_ptr_equal(panel->next, label);
    assert_ptr_equal(label->next, button);
    assert_null(button->next);
    
    // Test removing components
    RemoveComponent(panel, label);
    assert_ptr_equal(panel->next, button);
    assert_null(label->next);
    
    // Cleanup
    FreeComponent(panel);
    FreeComponent(label);
}

// Dialogue node creation tests
static void test_dialogue_node_creation(void **state) {
    TestState* testState = (TestState*)*state;
    
    // Create nodes
    testState->rootNode = CreateDialogueNode("root", "Root Node");
    testState->childNode = CreateDialogueNode("child", "Child Node");
    
    assert_non_null(testState->rootNode);
    assert_non_null(testState->childNode);
    assert_string_equal(testState->rootNode->id, "root");
    assert_string_equal(testState->rootNode->text, "Root Node");
    assert_null(testState->rootNode->choices);
    assert_int_equal(testState->rootNode->choiceCount, 0);
    
    // Test adding choices
    AddChoice(testState->rootNode, testState->childNode);
    assert_int_equal(testState->rootNode->choiceCount, 1);
    assert_ptr_equal(testState->rootNode->choices[0], testState->childNode);
    
    // Test node callbacks
    SetNodeCallbacks(testState->rootNode, test_callback, test_callback, testState);
    assert_non_null(testState->rootNode->onEnter);
    assert_non_null(testState->rootNode->onExit);
    assert_ptr_equal(testState->rootNode->userData, testState);
    
    // Call the callbacks to test
    testState->callbackCounter = 0;
    testState->rootNode->onEnter(testState->rootNode->userData);
    assert_int_equal(testState->callbackCounter, 1);
    testState->rootNode->onExit(testState->rootNode->userData);
    assert_int_equal(testState->callbackCounter, 2);
    
    // Cleanup handled by teardown_dialogue_nodes
}

// Setup/teardown for dialogue manager tests
static int setup_dialogue_nodes(void **state) {
    setup(state);
    TestState* testState = (TestState*)*state;
    
    testState->rootNode = CreateDialogueNode("root", "Root Node");
    testState->childNode = CreateDialogueNode("child", "Child Node");
    AddChoice(testState->rootNode, testState->childNode);
    
    return 0;
}

static int teardown_dialogue_nodes(void **state) {
    TestState* testState = (TestState*)*state;
    
    if (testState->rootNode != NULL) {
        if (testState->rootNode->choices != NULL) {
            free(testState->rootNode->choices);
        }
        free(testState->rootNode);
    }
    
    if (testState->childNode != NULL) {
        free(testState->childNode);
    }
    
    if (testState->manager != NULL) {
        free(testState->manager);
    }
    
    teardown(state);
    return 0;
}

// Dialogue manager tests
static void test_dialogue_manager_creation(void **state) {
    TestState* testState = (TestState*)*state;
    
    // Create manager
    testState->manager = CreateDialogueManager(testState->rootNode);
    
    assert_non_null(testState->manager);
    assert_ptr_equal(testState->manager->rootNode, testState->rootNode);
    assert_ptr_equal(testState->manager->currentNode, testState->rootNode);
    assert_true(testState->manager->isActive);
}

static void test_node_transition(void **state) {
    TestState* testState = (TestState*)*state;
    
    // Make sure manager is set up
    assert_non_null(testState->manager);
    
    // Test node transition
    TransitionToNode(testState->manager, "child");
    assert_ptr_equal(testState->manager->currentNode, testState->childNode);
    
    TransitionToNode(testState->manager, "root");
    assert_ptr_equal(testState->manager->currentNode, testState->rootNode);
}

// Component properties tests
static void test_component_properties(void **state) {
    // Create component
    RayDialComponent* button = CreateButton(
        (Rectangle){100, 100, 200, 50},
        "Test Button",
        test_callback,
        *state
    );
    
    // Test visible/enabled
    assert_true(button->visible);
    assert_true(button->enabled);
    
    SetComponentVisible(button, false);
    assert_false(button->visible);
    
    SetComponentEnabled(button, false);
    assert_false(button->enabled);
    
    // Cleanup
    FreeComponent(button);
}

// Edge case tests
static void test_null_parameters(void **state) {
    // Test behavior with NULL parameters
    
    // These should not crash but return NULL
    RayDialComponent* nullButton = CreateButton((Rectangle){0}, NULL, NULL, NULL);
    assert_non_null(nullButton);  // Should still create a button but with empty text
    
    RayDialNode* nullNode = CreateDialogueNode(NULL, NULL);
    assert_non_null(nullNode);    // Should create a node with empty strings
    
    // These should be safe no-ops
    FreeComponent(NULL);
    AddComponent(NULL, NULL);
    RemoveComponent(NULL, NULL);
    
    // Cleanup
    FreeComponent(nullButton);
    free(nullNode);
}

int main(void) {
    // Initialize raylib (minimal window for testing)
    const int screenWidth = 640;
    const int screenHeight = 480;
    InitWindow(screenWidth, screenHeight, "Raydial Test Suite (cmocka)");
    
    // Define test groups
    const struct CMUnitTest component_tests[] = {
        cmocka_unit_test(test_button_creation),
        cmocka_unit_test(test_label_creation),
        cmocka_unit_test(test_panel_creation),
        cmocka_unit_test(test_component_hierarchy),
        cmocka_unit_test(test_component_properties),
    };
    
    const struct CMUnitTest dialogue_tests[] = {
        cmocka_unit_test_setup_teardown(test_dialogue_node_creation, setup, teardown_dialogue_nodes),
        cmocka_unit_test_setup_teardown(test_dialogue_manager_creation, setup_dialogue_nodes, teardown_dialogue_nodes),
        cmocka_unit_test_setup_teardown(test_node_transition, setup_dialogue_nodes, teardown_dialogue_nodes),
    };
    
    const struct CMUnitTest edge_tests[] = {
        cmocka_unit_test(test_null_parameters),
    };
    
    // Run test groups
    printf("\n==== COMPONENT TESTS ====\n");
    int component_fails = cmocka_run_group_tests(component_tests, setup, teardown);
    
    printf("\n==== DIALOGUE TESTS ====\n");
    int dialogue_fails = cmocka_run_group_tests(dialogue_tests, NULL, NULL);
    
    printf("\n==== EDGE CASE TESTS ====\n");
    int edge_fails = cmocka_run_group_tests(edge_tests, setup, teardown);
    
    // Show visual results
    int failed = component_fails + dialogue_fails + edge_fails;
    
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            if (failed == 0) {
                DrawText("✅ ALL TESTS PASSED!", 190, 200, 20, GREEN);
            } else {
                DrawText(TextFormat("❌ %d TESTS FAILED!", failed), 190, 200, 20, RED);
            }
            
            DrawText("Press ESC to exit", 250, 240, 20, DARKGRAY);
        EndDrawing();
    }
    
    // Close window
    CloseWindow();
    
    return failed ? 1 : 0;
} 