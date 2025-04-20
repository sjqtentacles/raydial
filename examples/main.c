#include <raylib.h>
#include <raydial.h>

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "RayDial Example");
    SetTargetFPS(60);

    // Create a dialogue box
    Rectangle boxBounds = { 100, 100, 600, 200 };
    RayDialogueBox dialog = CreateDialogueBox(
        boxBounds,
        "Welcome!",
        "This is a simple dialogue box example using RayDial.\nClick to change the message."
    );

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw and update the dialogue box
        DrawDialogueBox(&dialog);
        UpdateDialogueBox(&dialog);

        // Check if the dialogue box was clicked
        if (IsDialogueBoxClicked(&dialog)) {
            // You can add logic here to handle the click
            // For example, change the message
            dialog.message = "You clicked the dialogue box!";
        }

        // if clicked elsewhere other than the dialogue box, close it
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(GetMousePosition(), dialog.bounds)) {
            dialog.finished = true;
            // close the window
            EndDrawing();
            CloseWindow();
            return 0;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
