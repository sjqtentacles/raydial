#include "raylib.h"

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Generate Portraits");
    SetTargetFPS(60);

    // Create render texture for portraits
    RenderTexture2D portrait = LoadRenderTexture(200, 200);

    // Generate neutral portrait
    BeginTextureMode(portrait);
        ClearBackground(RAYWHITE);
        // Draw face
        DrawCircle(100, 100, 80, LIGHTGRAY);
        // Draw eyes
        DrawCircle(70, 80, 10, BLACK);
        DrawCircle(130, 80, 10, BLACK);
        // Draw mouth (neutral)
        DrawLine(70, 130, 130, 130, BLACK);
    EndTextureMode();
    Image portraitImage = LoadImageFromTexture(portrait.texture);
    ExportImage(portraitImage, "../examples/resources/portrait_neutral.png");
    UnloadImage(portraitImage);

    // Generate happy portrait
    BeginTextureMode(portrait);
        ClearBackground(RAYWHITE);
        // Draw face
        DrawCircle(100, 100, 80, LIGHTGRAY);
        // Draw eyes
        DrawCircle(70, 80, 10, BLACK);
        DrawCircle(130, 80, 10, BLACK);
        // Draw mouth (smile)
        DrawCircle(100, 130, 20, BLACK);
        DrawCircle(100, 130, 15, LIGHTGRAY);
    EndTextureMode();
    portraitImage = LoadImageFromTexture(portrait.texture);
    ExportImage(portraitImage, "../examples/resources/portrait_happy.png");
    UnloadImage(portraitImage);

    // Generate angry portrait
    BeginTextureMode(portrait);
        ClearBackground(RAYWHITE);
        // Draw face
        DrawCircle(100, 100, 80, LIGHTGRAY);
        // Draw eyes
        DrawCircle(70, 80, 10, BLACK);
        DrawCircle(130, 80, 10, BLACK);
        // Draw eyebrows
        DrawLine(50, 70, 90, 70, BLACK);
        DrawLine(110, 70, 150, 70, BLACK);
        // Draw mouth (frown)
        DrawCircle(100, 130, 20, BLACK);
        DrawCircle(100, 130, 15, LIGHTGRAY);
    EndTextureMode();
    portraitImage = LoadImageFromTexture(portrait.texture);
    ExportImage(portraitImage, "../examples/resources/portrait_angry.png");
    UnloadImage(portraitImage);

    // Generate sad portrait
    BeginTextureMode(portrait);
        ClearBackground(RAYWHITE);
        // Draw face
        DrawCircle(100, 100, 80, LIGHTGRAY);
        // Draw eyes
        DrawCircle(70, 80, 10, BLACK);
        DrawCircle(130, 80, 10, BLACK);
        // Draw mouth (sad)
        DrawCircle(100, 150, 20, BLACK);
        DrawCircle(100, 150, 15, LIGHTGRAY);
    EndTextureMode();
    portraitImage = LoadImageFromTexture(portrait.texture);
    ExportImage(portraitImage, "../examples/resources/portrait_sad.png");
    UnloadImage(portraitImage);

    // Cleanup
    UnloadRenderTexture(portrait);
    CloseWindow();

    return 0;
} 