#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <string.h>

// Counter state
typedef struct {
    int counter;
} CounterState;

// Button callbacks
void OnIncrement(void* userData) {
    CounterState* state = (CounterState*)userData;
    state->counter++;
    printf("INCREMENT CALLED: Counter: %d\n", state->counter);
}

void OnDecrement(void* userData) {
    CounterState* state = (CounterState*)userData;
    if (state->counter > 0) {
        state->counter--;
    }
    printf("DECREMENT CALLED: Counter: %d\n", state->counter);
}

void OnReset(void* userData) {
    CounterState* state = (CounterState*)userData;
    state->counter = 0;
    printf("RESET CALLED: Counter reset to 0\n");
}

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Counter Example");
    SetTargetFPS(60);

    // Initialize state
    CounterState state = { .counter = 0 };

    // Create dialogue node
    RayDialNode* rootNode = CreateDialogueNode("root", "Counter Example");
    
    // Create dialogue manager
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    // Create UI panel
    RayDialComponent* panel = CreatePanel(
        (Rectangle){ 150, 100, 500, 400 },
        RAYWHITE
    );
    
    // Create title
    RayDialComponent* title = CreateLabel(
        (Rectangle){ 170, 120, 460, 40 },
        "Interactive Counter",
        true
    );
    
    // Create description
    RayDialComponent* description = CreateLabel(
        (Rectangle){ 170, 170, 460, 100 },
        "This example demonstrates button functionality with proper text wrapping. Use the buttons below to change the counter value.",
        true
    );
    
    // Create counter display
    RayDialComponent* counterDisplay = CreateLabel(
        (Rectangle){ 170, 280, 460, 40 },
        "Counter: 0",
        true
    );
    
    // Create buttons
    RayDialComponent* incrementButton = CreateButton(
        (Rectangle){ 170, 340, 150, 50 },
        "Increment",
        OnIncrement,
        &state
    );
    
    RayDialComponent* decrementButton = CreateButton(
        (Rectangle){ 340, 340, 150, 50 },
        "Decrement",
        OnDecrement,
        &state
    );
    
    RayDialComponent* resetButton = CreateButton(
        (Rectangle){ 170, 410, 320, 50 },
        "Reset Counter",
        OnReset,
        &state
    );
    
    // Assemble UI
    AddComponent(panel, title);
    AddComponent(panel, description);
    AddComponent(panel, counterDisplay);
    AddComponent(panel, incrementButton);
    AddComponent(panel, decrementButton);
    AddComponent(panel, resetButton);
    
    // Set root node component
    rootNode->components = panel;

    // Main game loop
    while (!WindowShouldClose()) {
        // Debug mouse information
        Vector2 mousePos = GetMousePosition();
        bool isMousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
        
        // Update
        UpdateDialogueManager(manager);
        
        // Update counter display
        char counterText[32];
        snprintf(counterText, sizeof(counterText), "Counter: %d", state.counter);
        ((RayDialLabelData*)counterDisplay->data)->text = counterText;
        
        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawDialogueManager(manager);
            DrawText("Press ESC to exit", 10, screenHeight - 30, 20, DARKGRAY);
            
            // Debug text
            DrawText(TextFormat("Mouse: %.0f, %.0f | Clicked: %d", 
                      mousePos.x, mousePos.y, isMousePressed), 
                      10, 10, 20, BLACK);
        EndDrawing();
    }
    
    // Cleanup
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 