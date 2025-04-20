#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Gallery state
typedef struct {
    Color* colors;           // We'll use colored rectangles instead of images
    int imageCount;
    char** imageTitles;
    char** imageDescriptions;
    int currentImageIndex;
    float scrollPosition;
} GalleryState;

// Button callbacks
void OnNextImage(void* userData) {
    GalleryState* state = (GalleryState*)userData;
    if (state->currentImageIndex < state->imageCount - 1) {
        state->currentImageIndex++;
        state->scrollPosition = 0; // Reset scroll position for new image
        printf("Showing image %d of %d\n", state->currentImageIndex + 1, state->imageCount);
    }
}

void OnPrevImage(void* userData) {
    GalleryState* state = (GalleryState*)userData;
    if (state->currentImageIndex > 0) {
        state->currentImageIndex--;
        state->scrollPosition = 0; // Reset scroll position for new image
        printf("Showing image %d of %d\n", state->currentImageIndex + 1, state->imageCount);
    }
}

// Load gallery images
GalleryState LoadGallery() {
    GalleryState state;
    
    // Create colors for our "portraits"
    state.imageCount = 4;
    state.colors = (Color*)malloc(sizeof(Color) * state.imageCount);
    state.imageTitles = (char**)malloc(sizeof(char*) * state.imageCount);
    state.imageDescriptions = (char**)malloc(sizeof(char*) * state.imageCount);
    
    // Set colors for each "portrait"
    state.colors[0] = YELLOW;      // Happy
    state.colors[1] = LIGHTGRAY;   // Neutral
    state.colors[2] = BLUE;        // Sad
    state.colors[3] = RED;         // Angry
    
    // Set titles and descriptions
    const char* titles[] = {
        "Happy Portrait (Yellow)",
        "Neutral Portrait (Gray)",
        "Sad Portrait (Blue)",
        "Angry Portrait (Red)"
    };
    
    const char* descriptions[] = {
        "This is the happy portrait, represented by yellow.\n\n"
        "Yellow is often associated with happiness, optimism, and positive energy. "
        "This color can be used to represent cheerful character expressions.",
        
        "This is the neutral portrait, represented by gray.\n\n"
        "Gray represents neutrality and balance. It's perfect for situations "
        "where the character isn't expressing any particular emotion.",
        
        "This is the sad portrait, represented by blue.\n\n"
        "Blue is often associated with sadness, melancholy, and calm. "
        "It works well for representing a character who is feeling down.",
        
        "This is the angry portrait, represented by red.\n\n"
        "Red represents anger, passion, and intensity. It's an effective color "
        "for showing that a character is upset or agitated."
    };
    
    for (int i = 0; i < state.imageCount; i++) {
        state.imageTitles[i] = (char*)malloc(64);
        strcpy(state.imageTitles[i], titles[i]);
        
        state.imageDescriptions[i] = (char*)malloc(512);
        strcpy(state.imageDescriptions[i], descriptions[i]);
    }
    
    state.currentImageIndex = 0;
    state.scrollPosition = 0;
    
    return state;
}

// Unload gallery resources
void UnloadGallery(GalleryState* state) {
    for (int i = 0; i < state->imageCount; i++) {
        free(state->imageTitles[i]);
        free(state->imageDescriptions[i]);
    }
    
    free(state->colors);
    free(state->imageTitles);
    free(state->imageDescriptions);
}

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Image Gallery Example");
    SetTargetFPS(60);
    
    // Initialize image gallery
    GalleryState gallery = LoadGallery();
    
    // Create dialogue node
    RayDialNode* rootNode = CreateDialogueNode("root", "Image Gallery Example");
    
    // Create dialogue manager
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    // Create main UI panel
    RayDialComponent* panel = CreatePanel(
        (Rectangle){ 50, 50, 700, 500 },
        RAYWHITE
    );
    
    // Create title
    RayDialComponent* title = CreateLabel(
        (Rectangle){ 70, 70, 660, 40 },
        "Color Portrait Gallery",
        true
    );
    
    // Create image display area
    RayDialComponent* imagePanel = CreatePanel(
        (Rectangle){ 70, 120, 300, 300 },
        LIGHTGRAY
    );
    
    // Create description panel instead of scroll area
    RayDialComponent* descriptionPanel = CreatePanel(
        (Rectangle){ 390, 120, 340, 300 },
        LIGHTGRAY
    );
    
    // Create image title label
    RayDialComponent* imageTitle = CreateLabel(
        (Rectangle){ 400, 130, 320, 40 },
        gallery.imageTitles[0],
        true
    );
    
    // Create image description label
    RayDialComponent* imageDescription = CreateLabel(
        (Rectangle){ 400, 180, 320, 220 },  // Adjusted height to fit within panel
        gallery.imageDescriptions[0],
        true
    );
    
    // Create navigation buttons
    RayDialComponent* prevButton = CreateButton(
        (Rectangle){ 70, 430, 150, 40 },
        "Previous Color",
        OnPrevImage,
        &gallery
    );
    
    RayDialComponent* nextButton = CreateButton(
        (Rectangle){ 230, 430, 150, 40 },
        "Next Color",
        OnNextImage,
        &gallery
    );
    
    // Create info label
    RayDialComponent* infoLabel = CreateLabel(
        (Rectangle){ 390, 430, 340, 40 },
        "Use the buttons to navigate between colors",
        true
    );
    
    // Add components to the description panel
    AddComponent(descriptionPanel, imageTitle);
    AddComponent(descriptionPanel, imageDescription);
    
    // Add components to the main panel
    AddComponent(panel, title);
    AddComponent(panel, imagePanel);
    AddComponent(panel, descriptionPanel);
    AddComponent(panel, prevButton);
    AddComponent(panel, nextButton);
    AddComponent(panel, infoLabel);
    
    // Set the panel as the root node's component
    rootNode->components = panel;
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateDialogueManager(manager);
        
        // Update image and description based on current selection
        char titleBuffer[64];
        sprintf(titleBuffer, "%s (%d of %d)", 
            gallery.imageTitles[gallery.currentImageIndex],
            gallery.currentImageIndex + 1, 
            gallery.imageCount);
        ((RayDialLabelData*)imageTitle->data)->text = titleBuffer;
        ((RayDialLabelData*)imageDescription->data)->text = gallery.imageDescriptions[gallery.currentImageIndex];
        
        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);
            
            // Draw manager (UI components)
            DrawDialogueManager(manager);
            
            // Draw current "portrait" as a colored rectangle
            DrawRectangle(
                imagePanel->bounds.x + 50,
                imagePanel->bounds.y + 50,
                200,
                200,
                gallery.colors[gallery.currentImageIndex]
            );
            
            // Add a facial expression to the colored rectangle
            if (gallery.currentImageIndex == 0) {  // Happy
                DrawCircle(imagePanel->bounds.x + 150, imagePanel->bounds.y + 150, 100, gallery.colors[0]);
                DrawCircle(imagePanel->bounds.x + 120, imagePanel->bounds.y + 120, 15, BLACK);
                DrawCircle(imagePanel->bounds.x + 180, imagePanel->bounds.y + 120, 15, BLACK);
                DrawText(")", imagePanel->bounds.x + 140, imagePanel->bounds.y + 170, 40, BLACK);
            } 
            else if (gallery.currentImageIndex == 1) {  // Neutral
                DrawCircle(imagePanel->bounds.x + 150, imagePanel->bounds.y + 150, 100, gallery.colors[1]);
                DrawCircle(imagePanel->bounds.x + 120, imagePanel->bounds.y + 120, 15, BLACK);
                DrawCircle(imagePanel->bounds.x + 180, imagePanel->bounds.y + 120, 15, BLACK);
                DrawRectangle(imagePanel->bounds.x + 130, imagePanel->bounds.y + 180, 40, 5, BLACK);
            }
            else if (gallery.currentImageIndex == 2) {  // Sad
                DrawCircle(imagePanel->bounds.x + 150, imagePanel->bounds.y + 150, 100, gallery.colors[2]);
                DrawCircle(imagePanel->bounds.x + 120, imagePanel->bounds.y + 120, 15, BLACK);
                DrawCircle(imagePanel->bounds.x + 180, imagePanel->bounds.y + 120, 15, BLACK);
                DrawText("(", imagePanel->bounds.x + 140, imagePanel->bounds.y + 170, 40, BLACK);
            }
            else if (gallery.currentImageIndex == 3) {  // Angry
                DrawCircle(imagePanel->bounds.x + 150, imagePanel->bounds.y + 150, 100, gallery.colors[3]);
                DrawCircle(imagePanel->bounds.x + 120, imagePanel->bounds.y + 120, 15, BLACK);
                DrawCircle(imagePanel->bounds.x + 180, imagePanel->bounds.y + 120, 15, BLACK);
                DrawLine(imagePanel->bounds.x + 130, imagePanel->bounds.y + 180, 
                         imagePanel->bounds.x + 170, imagePanel->bounds.y + 190, BLACK);
            }
            
            // Draw help text
            DrawText("Press ESC to exit", 10, screenHeight - 30, 20, DARKGRAY);
        EndDrawing();
    }
    
    // Cleanup
    UnloadGallery(&gallery);
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 