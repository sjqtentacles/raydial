#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <string.h>

// Structure to track conversation state
typedef struct {
    const char** dialogues;
    int dialogueCount;
    int currentIndex;
    RayDialComponent* dialogueLabel;
} DialogueState;

// Example dialogues
const char* exampleDialogues[] = {
    "Welcome to the Enter Dialogue Example!",
    "This example shows how to progress through dialogue using the Enter key.",
    "Press Enter to continue to the next line of dialogue.",
    "You can also use the Space key if you prefer.",
    "The dialogue will loop back to the beginning when it reaches the end.",
    "Thank you for trying out this example!"
};

// Update dialogue based on keyboard input
void UpdateDialogue(DialogueState* state) {
    // Check for Enter or Space key press
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        // Move to next dialogue
        state->currentIndex = (state->currentIndex + 1) % state->dialogueCount;
        
        // Update the label text
        ((RayDialLabelData*)state->dialogueLabel->data)->text = state->dialogues[state->currentIndex];
    }
}

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Enter Dialogue Example");
    SetTargetFPS(60);

    // Initialize dialogue state
    DialogueState state = {
        .dialogues = exampleDialogues,
        .dialogueCount = sizeof(exampleDialogues) / sizeof(exampleDialogues[0]),
        .currentIndex = 0
    };

    // Create dialogue manager and root node
    RayDialNode* rootNode = CreateDialogueNode("root", "Enter Dialogue Example");
    RayDialManager* manager = CreateDialogueManager(rootNode);

    // Create UI components
    RayDialComponent* panel = CreatePanel(
        (Rectangle){ 100, 100, 600, 250 },
        LIGHTGRAY
    );

    // Create dialogue label
    state.dialogueLabel = CreateLabel(
        (Rectangle){ 120, 120, 560, 180 },
        state.dialogues[0],
        true  // Enable text wrapping
    );

    // Create instruction label
    RayDialComponent* instructionLabel = CreateLabel(
        (Rectangle){ 120, 320, 560, 20 },
        "Press Enter or Space to continue",
        false
    );

    // Assemble UI
    AddComponent(panel, state.dialogueLabel);
    AddComponent(panel, instructionLabel);
    rootNode->components = panel;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update dialogue based on keyboard input
        UpdateDialogue(&state);

        // Update dialogue manager
        UpdateDialogueManager(manager);

        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawDialogueManager(manager);
            
            // Draw current dialogue index
            char indexText[32];
            snprintf(indexText, sizeof(indexText), "Dialogue %d/%d", 
                     state.currentIndex + 1, state.dialogueCount);
            DrawText(indexText, 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Cleanup
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 