#include "raylib.h"
#include "raydial.h"
#include <stddef.h>

int main(void) {
    // Initialize raylib window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raydial Basic Example");
    SetTargetFPS(60);

    // Create a root dialogue node
    RayDialNode* rootNode = CreateDialogueNode("root", "Welcome to Raydial!");
    
    // Create a dialogue manager
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    // Create UI components for the root node
    RayDialComponent* panel = CreatePanel(
        (Rectangle){ 100, 100, 600, 250 },
        RAYWHITE
    );
    
    RayDialComponent* titleLabel = CreateLabel(
        (Rectangle){ 120, 120, 560, 40 },
        "Welcome to Raydial!",
        true  // Enable text wrapping
    );
    
    RayDialComponent* messageLabel = CreateLabel(
        (Rectangle){ 120, 170, 560, 150 },  // Increased height for more spacing
        "This is a basic example of using Raydial to create dialogue UIs.\n\nText will now properly wrap within the bounds of this label component, preventing overflow and making the UI look much cleaner.",
        true  // Enable text wrapping
    );
    
    // Add components to the panel
    AddComponent(panel, titleLabel);
    AddComponent(panel, messageLabel);
    
    // Set the panel as the root node's component
    rootNode->components = panel;
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateDialogueManager(manager);
        
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawDialogueManager(manager);
            DrawText("Press ESC to exit", 10, screenHeight - 30, 20, DARKGRAY);
        EndDrawing();
    }
    
    // Cleanup
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 