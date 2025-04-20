#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Types of text animation
typedef enum {
    TEXT_SPEED_SLOW = 1,
    TEXT_SPEED_NORMAL = 3,
    TEXT_SPEED_FAST = 6
} TextSpeed;

// Animation state structure
typedef struct {
    const char* fullText;        // Complete text to display
    char* displayText;           // Current displayed text (partial)
    int textLength;              // Length of full text
    int currentLength;           // Currently displayed length
    TextSpeed speed;             // Animation speed
    bool animationComplete;      // Flag for animation completion
    int framesCounter;           // Frame counter for animation timing
} AnimatedText;

// Function to initialize animated text
AnimatedText InitAnimatedText(const char* text, TextSpeed speed) {
    AnimatedText anim;
    anim.fullText = text;
    anim.textLength = strlen(text);
    anim.displayText = (char*)malloc(anim.textLength + 1);
    anim.displayText[0] = '\0';
    anim.currentLength = 0;
    anim.speed = speed;
    anim.animationComplete = false;
    anim.framesCounter = 0;
    return anim;
}

// Function to update animated text
void UpdateAnimatedText(AnimatedText* anim) {
    if (anim->animationComplete) return;
    
    anim->framesCounter++;
    
    // Update text based on speed
    if (anim->framesCounter >= (60 / (anim->speed * 5))) {
        anim->framesCounter = 0;
        
        if (anim->currentLength < anim->textLength) {
            // Copy next character
            anim->displayText[anim->currentLength] = anim->fullText[anim->currentLength];
            anim->displayText[anim->currentLength + 1] = '\0';
            anim->currentLength++;
            
            // Play a typing sound (simulated with console output)
            if (anim->currentLength % 5 == 0) {
                printf("*click*\n");
            }
        } else {
            anim->animationComplete = true;
        }
    }
}

// Function to get current display text
const char* GetAnimatedText(AnimatedText* anim) {
    return anim->displayText;
}

// Function to free animated text resources
void FreeAnimatedText(AnimatedText* anim) {
    free(anim->displayText);
    anim->displayText = NULL;
}

// Button callbacks
void OnSetSpeedSlow(void* userData) {
    AnimatedText* anim = (AnimatedText*)userData;
    anim->speed = TEXT_SPEED_SLOW;
    printf("Text speed set to SLOW\n");
}

void OnSetSpeedNormal(void* userData) {
    AnimatedText* anim = (AnimatedText*)userData;
    anim->speed = TEXT_SPEED_NORMAL;
    printf("Text speed set to NORMAL\n");
}

void OnSetSpeedFast(void* userData) {
    AnimatedText* anim = (AnimatedText*)userData;
    anim->speed = TEXT_SPEED_FAST;
    printf("Text speed set to FAST\n");
}

void OnResetAnimation(void* userData) {
    AnimatedText* anim = (AnimatedText*)userData;
    anim->currentLength = 0;
    anim->displayText[0] = '\0';
    anim->animationComplete = false;
    printf("Animation reset\n");
}

void OnCompleteAnimation(void* userData) {
    AnimatedText* anim = (AnimatedText*)userData;
    strcpy(anim->displayText, anim->fullText);
    anim->currentLength = anim->textLength;
    anim->animationComplete = true;
    printf("Animation completed\n");
}

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Animated Text Example");
    SetTargetFPS(60);

    // Initialize animated text with a much longer text to demonstrate scrolling
    const char* dialogueText =
        "Once upon a time in a land far, far away...\n\n"
        "There lived a brave hero who embarked on a journey to save the kingdom "
        "from an ancient evil that had awakened after centuries of slumber.\n\n"
        "The hero faced many challenges along the way, but with determination "
        "and courage, continued forward despite all obstacles.\n\n"
        "As our hero ventured deeper into the mysterious forest, they encountered strange creatures "
        "and magical phenomena that defied explanation. Trees whispered secrets of old, and streams "
        "flowed with water that glowed under the moonlight.\n\n"
        "In a small clearing, the hero found an old hermit living in a hut made of twisted branches "
        "and moss. The hermit spoke of prophecies and destinies, of stars aligning and ancient powers "
        "stirring once more.\n\n"
        "'You have been chosen,' said the hermit, handing the hero a peculiar amulet. 'This will "
        "guide you through the darkness ahead. But beware, for where there is great power, there is "
        "also great danger.'\n\n"
        "The hero accepted the amulet, feeling its weight—both physical and metaphorical—in their palm. "
        "It pulsed with an inner light, as if alive with magic from ages past.\n\n"
        "'How will I know what to do when the time comes?' the hero asked.\n\n"
        "The hermit smiled mysteriously. 'The amulet will show you the way, but the choices you make "
        "will determine the fate of all. Trust your heart, for it knows truth that the mind cannot grasp.'\n\n"
        "With these cryptic words echoing in their mind, the hero continued the journey, now with an "
        "amulet that occasionally whispered guidance in languages both familiar and strange.\n\n"
        "Days turned into weeks as the hero traversed mountains so tall they seemed to pierce the heavens. "
        "They crossed deserts where the sand shifted like water, revealing ancient ruins beneath. They "
        "navigated through caverns that glowed with crystals of every color imaginable.\n\n"
        "Along the way, allies were made—a skilled archer with eyes that could spot a needle in a haystack "
        "from a hundred paces; a wise sage who could read the stars and predict storms; a jovial bard whose "
        "songs had the power to heal wounds and lift spirits.\n\n"
        "Together, they formed a band of unlikely friends united by a common purpose—to reach the heart of "
        "the ancient evil and put an end to its growing darkness before it consumed everything in its path.\n\n"
        "As they neared their destination, the skies grew darker, the air heavier with malice. Creatures of "
        "shadow stalked them, testing their resolve and unity. But for every challenge, they found strength "
        "in each other.\n\n"
        "Finally, they stood before a fortress of obsidian that seemed to devour light itself. The amulet "
        "glowed brighter than ever, pushing back the darkness just enough for them to find their way forward.\n\n"
        "'Remember why we're here,' the hero said to their companions. 'For the future of all.'\n\n"
        "And with that, they stepped into the unknown, ready to face whatever awaited them within the heart "
        "of darkness...\n\n"
        "TO BE CONTINUED...";
                               
    AnimatedText animText = InitAnimatedText(dialogueText, TEXT_SPEED_NORMAL);

    // Create dialogue node
    RayDialNode* rootNode = CreateDialogueNode("root", "Animated Text Example");
    
    // Create dialogue manager
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    // Create UI panel
    RayDialComponent* panel = CreatePanel(
        (Rectangle){ 100, 100, 600, 400 },
        RAYWHITE
    );
    
    // Create title
    RayDialComponent* title = CreateLabel(
        (Rectangle){ 120, 120, 560, 40 },
        "Animated Text Example - Now With Scrolling!",
        true
    );
    
    // Create animated text label - this will now be scrollable
    RayDialComponent* textLabel = CreateLabel(
        (Rectangle){ 120, 170, 560, 220 },
        "",  // Initially empty, will be updated with animated text
        true
    );
    
    // Create speed control buttons
    RayDialComponent* slowButton = CreateButton(
        (Rectangle){ 120, 400, 150, 40 },
        "Slow Speed",
        OnSetSpeedSlow,
        &animText
    );
    
    RayDialComponent* normalButton = CreateButton(
        (Rectangle){ 280, 400, 150, 40 },
        "Normal Speed",
        OnSetSpeedNormal,
        &animText
    );
    
    RayDialComponent* fastButton = CreateButton(
        (Rectangle){ 440, 400, 150, 40 },
        "Fast Speed",
        OnSetSpeedFast,
        &animText
    );
    
    // Create control buttons
    RayDialComponent* resetButton = CreateButton(
        (Rectangle){ 120, 450, 230, 40 },
        "Reset Animation",
        OnResetAnimation,
        &animText
    );
    
    RayDialComponent* completeButton = CreateButton(
        (Rectangle){ 360, 450, 230, 40 },
        "Complete Animation",
        OnCompleteAnimation,
        &animText
    );
    
    // Assemble UI
    AddComponent(panel, title);
    AddComponent(panel, textLabel);
    AddComponent(panel, slowButton);
    AddComponent(panel, normalButton);
    AddComponent(panel, fastButton);
    AddComponent(panel, resetButton);
    AddComponent(panel, completeButton);
    
    // Set root node component
    rootNode->components = panel;

    // Main game loop
    while (!WindowShouldClose()) {
        // Update animated text
        UpdateAnimatedText(&animText);
        
        // Update the text label with current animation state
        ((RayDialLabelData*)textLabel->data)->text = GetAnimatedText(&animText);
        
        // Update
        UpdateDialogueManager(manager);
        
        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);
            DrawDialogueManager(manager);
            
            // Draw additional info
            DrawText("Press ESC to exit", 10, screenHeight - 30, 20, DARKGRAY);
            DrawText("Use mouse wheel or arrow keys to scroll text", 10, screenHeight - 60, 20, DARKGRAY);
            
            // Draw animation status
            const char* status = animText.animationComplete ? "Animation complete" : "Animating...";
            DrawText(status, 10, 10, 20, DARKGRAY);
            
            // Draw current speed
            const char* speedText;
            switch(animText.speed) {
                case TEXT_SPEED_SLOW: speedText = "Speed: Slow"; break;
                case TEXT_SPEED_NORMAL: speedText = "Speed: Normal"; break;
                case TEXT_SPEED_FAST: speedText = "Speed: Fast"; break;
                default: speedText = "Speed: Unknown"; break;
            }
            DrawText(speedText, 10, 40, 20, DARKGRAY);
            
        EndDrawing();
    }
    
    // Cleanup
    FreeAnimatedText(&animText);
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 