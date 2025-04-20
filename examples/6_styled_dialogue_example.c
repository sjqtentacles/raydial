#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Character states (used for switching portraits)
#define CHARACTER_HAPPY 0
#define CHARACTER_NEUTRAL 1
#define CHARACTER_SAD 2
#define CHARACTER_ANGRY 3

// Example dialogue data with rich text formatting
typedef struct {
    const char* speakerName;
    const char* dialogueText;
    int portraitState;
    bool showOnRight;
} DialogueEntry;

// Dialogue advancement callback
void OnNextDialogue(void* userData);

// Structure to track conversation state
typedef struct {
    DialogueEntry* entries;
    int entryCount;
    int currentIndex;
    RayDialComponent* portraitDialogue;
} ConversationState;

// Create a basic emotional face based on state (same as in example 5)
void DrawEmotionalFace(Rectangle bounds, int emotionState) {
    Color faceColor;
    switch (emotionState) {
        case CHARACTER_HAPPY:
            faceColor = YELLOW;
            break;
        case CHARACTER_NEUTRAL:
            faceColor = LIGHTGRAY;
            break;
        case CHARACTER_SAD:
            faceColor = BLUE;
            break;
        case CHARACTER_ANGRY:
            faceColor = RED;
            break;
        default:
            faceColor = WHITE;
    }
    
    // Draw face
    DrawCircle(bounds.x + bounds.width/2, bounds.y + bounds.height/2, 
               fminf(bounds.width, bounds.height)/2.2f, faceColor);
               
    // Draw features based on emotion
    switch (emotionState) {
        case CHARACTER_HAPPY:
            // Happy face
            DrawCircle(bounds.x + bounds.width/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawCircle(bounds.x + bounds.width*2/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawText(")", bounds.x + bounds.width*0.45, bounds.y + bounds.height*0.6, 30, BLACK);
            break;
        case CHARACTER_NEUTRAL:
            // Neutral face
            DrawCircle(bounds.x + bounds.width/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawCircle(bounds.x + bounds.width*2/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawRectangle(bounds.x + bounds.width/3, bounds.y + bounds.height*0.7, 
                          bounds.width/3, 5, BLACK);
            break;
        case CHARACTER_SAD:
            // Sad face
            DrawCircle(bounds.x + bounds.width/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawCircle(bounds.x + bounds.width*2/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawText("(", bounds.x + bounds.width*0.45, bounds.y + bounds.height*0.6, 30, BLACK);
            break;
        case CHARACTER_ANGRY:
            // Angry face
            DrawCircle(bounds.x + bounds.width/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawCircle(bounds.x + bounds.width*2/3, bounds.y + bounds.height*0.4, 5, BLACK);
            DrawLine(bounds.x + bounds.width/3, bounds.y + bounds.height*0.7, 
                     bounds.x + bounds.width*2/3, bounds.y + bounds.height*0.75, BLACK);
            // Angry eyebrows
            DrawLine(bounds.x + bounds.width/4, bounds.y + bounds.height*0.3, 
                     bounds.x + bounds.width*0.4, bounds.y + bounds.height*0.35, BLACK);
            DrawLine(bounds.x + bounds.width*3/4, bounds.y + bounds.height*0.3, 
                     bounds.x + bounds.width*0.6, bounds.y + bounds.height*0.35, BLACK);
            break;
    }
}

// Progress to the next dialogue entry
void OnNextDialogue(void* userData) {
    ConversationState* conversation = (ConversationState*)userData;
    
    conversation->currentIndex++;
    if (conversation->currentIndex >= conversation->entryCount) {
        // Loop back to the beginning when we reach the end
        conversation->currentIndex = 0;
    }
    
    // Get the current dialogue entry
    DialogueEntry* currentEntry = &conversation->entries[conversation->currentIndex];
    
    // Update the portrait dialogue component with stylized text
    SetPortraitDialogueSpeaker(conversation->portraitDialogue, currentEntry->speakerName);
    SetPortraitDialogueStyledText(conversation->portraitDialogue, currentEntry->dialogueText);
    SetPortraitDialogueColor(conversation->portraitDialogue, 
        currentEntry->portraitState == CHARACTER_HAPPY ? YELLOW :
        currentEntry->portraitState == CHARACTER_NEUTRAL ? LIGHTGRAY :
        currentEntry->portraitState == CHARACTER_SAD ? BLUE : RED);
    SetPortraitDialoguePosition(conversation->portraitDialogue, currentEntry->showOnRight);
    
    printf("Showing styled dialogue %d of %d: %s\n", 
           conversation->currentIndex + 1, 
           conversation->entryCount,
           currentEntry->speakerName);
}

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Styled Dialogue Example");
    SetTargetFPS(60);
    
    // Initialize dialogue entries with rich text styling
    DialogueEntry dialogues[] = {
        {"Knight", "Hello adventurer! I'm [color=yellow]Sir Galahad[/color], knight of the realm.", CHARACTER_HAPPY, false},
        
        {"Wizard", "I am [color=blue]Merlin[/color], a [size=large]wizard[/size] of great power.", CHARACTER_NEUTRAL, true},
        
        {"Knight", "We need your help to defeat the [color=red][b]dragon[/b][/color] that threatens our kingdom!", CHARACTER_NEUTRAL, false},
        
        {"Wizard", "This quest will be [size=small]dangerous[/size] but [size=large]rewarding[/size]! Are you prepared?", CHARACTER_SAD, true},
        
        {"Knight", "Take this [color=yellow]golden[/color] sword. It will aid you on your journey.", CHARACTER_HAPPY, false},
        
        {"Wizard", "[color=red][size=large]WARNING![/size][/color] The dragon breathes fire that can melt even the strongest armor.", CHARACTER_ANGRY, true},
        
        {"Knight", "But with [color=blue]courage[/color] and [color=green]wisdom[/color], you shall prevail!", CHARACTER_NEUTRAL, false},
        
        {"Wizard", "Remember: The dragon's weakness is its [color=purple][b]crystal heart[/b][/color]. Aim for it!", CHARACTER_NEUTRAL, true},
        
        {"Knight", "May the gods bless your journey, [size=large]brave hero[/size]!", CHARACTER_HAPPY, false}
    };
    
    // Initialize conversation state
    ConversationState conversation;
    conversation.entries = dialogues;
    conversation.entryCount = sizeof(dialogues) / sizeof(DialogueEntry);
    conversation.currentIndex = 0;
    
    // Create dialogue node
    RayDialNode* rootNode = CreateDialogueNode("root", "Styled Dialogue Example");
    
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
        "Styled Dialogue Text Example",
        true
    );
    
    // Create portrait dialogue component
    DialogueEntry* firstEntry = &conversation.entries[0];
    RayDialComponent* portraitDialogue = CreatePortraitDialogue(
        (Rectangle){ 70, 120, 660, 300 },
        firstEntry->speakerName,
        "Plain text version",  // Will be replaced with styled text
        firstEntry->portraitState == CHARACTER_HAPPY ? YELLOW :
        firstEntry->portraitState == CHARACTER_NEUTRAL ? LIGHTGRAY :
        firstEntry->portraitState == CHARACTER_SAD ? BLUE : RED
    );
    
    // Set styled text for the dialogue
    SetPortraitDialogueStyledText(portraitDialogue, firstEntry->dialogueText);
    
    // Store the portrait dialogue component in the conversation state
    conversation.portraitDialogue = portraitDialogue;
    
    // Set initial portrait position
    SetPortraitDialoguePosition(portraitDialogue, firstEntry->showOnRight);
    
    // Create next dialogue button
    RayDialComponent* nextButton = CreateButton(
        (Rectangle){ 325, 430, 150, 40 },
        "Next Dialogue",
        OnNextDialogue,
        &conversation
    );
    
    // Create info label
    RayDialComponent* infoLabel = CreateLabel(
        (Rectangle){ 70, 480, 660, 30 },
        "This example demonstrates styled text with colors, sizes, and formatting",
        true
    );
    
    // Add components to the panel
    AddComponent(panel, title);
    AddComponent(panel, portraitDialogue);
    AddComponent(panel, nextButton);
    AddComponent(panel, infoLabel);
    
    // Set the panel as the root node's component
    rootNode->components = panel;
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateDialogueManager(manager);
        
        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);
            
            // Draw manager (UI components)
            DrawDialogueManager(manager);
            
            // Custom drawing for the portrait (override the default solid color)
            DialogueEntry* currentEntry = &conversation.entries[conversation.currentIndex];
            Rectangle portraitBounds;
            if (currentEntry->showOnRight) {
                portraitBounds = (Rectangle){ 
                    portraitDialogue->bounds.x + portraitDialogue->bounds.width - 110, 
                    portraitDialogue->bounds.y + 10, 
                    100, 
                    100 
                };
            } else {
                portraitBounds = (Rectangle){ 
                    portraitDialogue->bounds.x + 10, 
                    portraitDialogue->bounds.y + 10, 
                    100, 
                    100 
                };
            }
            
            // Draw the emotional face
            DrawEmotionalFace(portraitBounds, currentEntry->portraitState);
            
            // Draw formatting legend
            DrawText("Formatting Tags:", 70, 520, 20, BLACK);
            DrawText("[color=red]text[/color]", 70, 550, 16, RED);
            DrawText("[size=large]text[/size]", 270, 550, 16, BLACK);
            DrawText("[b]text[/b] (bold)", 470, 550, 16, BLACK);
            
            // Draw help text
            DrawText("Press ESC to exit", 10, screenHeight - 30, 20, DARKGRAY);
        EndDrawing();
    }
    
    // Cleanup
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 