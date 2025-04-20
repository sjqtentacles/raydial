#include "raylib.h"
#include "raydial.h"
#include "raydial_i18n.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Language switching callback
void OnSwitchLanguage(void* userData);

// Structure to hold application state
typedef struct {
    RayDialI18N* i18n;
    RayDialComponent* dialogueComponent;
    RayDialComponent* languageLabel;
    RayDialComponent* greetingLabel;
    RayDialComponent* title;
    RayDialComponent* switchButton;
    int currentLanguageIndex;
    const char* availableLanguages[3]; // Language codes for English, Spanish, and Greek
    Font originalDefaultFont; // Store the original default font
    Font greekFont;           // Font loaded with Greek glyphs
    bool greekFontLoaded;     // Flag if Greek font was loaded successfully
} AppState;

int main(void) {
    // Initialize window
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "RayDial Localization Example");
    SetTargetFPS(60);
    
    // Store the original default font
    Font originalDefaultFont = GetFontDefault();
    
    // Print current working directory for debugging
    const char* currentDir = GetWorkingDirectory();
    printf("Current working directory: %s\n", currentDir);
    
    // Initialize application state
    AppState state;
    state.originalDefaultFont = originalDefaultFont; // Store original font
    state.greekFontLoaded = false;
    
    // Define the characters needed for Greek text + common Latin/symbols
    const char* greekChars = "Παράδειγμα τοπικοποίησης Καλώς ήρθατε στη RayDial Αλλαγή γλώσσας Δείγμα διαλόγου Οδηγός Αυτό είναι ένα παράδειγμα [color=green]κειμένου[/color] με [size=large]μορφοποίηση[/size]. Τρέχουσα: Ελληνικά ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789 .,:;!?()[]{}<>\"'";
    int codepointCount = 0;
    int* codepoints = LoadCodepoints(greekChars, &codepointCount);
    printf("Attempting to load %d glyphs for Greek font.\n", codepointCount);
    
    // Try to load Greek font from system paths with specific codepoints
    const char* greekFontPaths[] = {
        "/System/Library/Fonts/Supplemental/Arial Unicode.ttf", // Common on macOS
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",      // Common on Linux
        "C:/Windows/Fonts/Arial.ttf",                           // Common on Windows (might lack Greek)
        "C:/Windows/Fonts/Tahoma.ttf",                           // Common on Windows
        NULL
    };
    
    for (int i = 0; greekFontPaths[i] != NULL; i++) {
        if (FileExists(greekFontPaths[i])) {
            // Load font with specific codepoints
            state.greekFont = LoadFontEx(greekFontPaths[i], 20, codepoints, codepointCount);
            if (state.greekFont.texture.id != 0) {
                state.greekFontLoaded = true;
                printf("Successfully loaded Greek font (%d glyphs) from: %s\n", state.greekFont.glyphCount, greekFontPaths[i]);
                break; // Stop searching once a font is loaded
            } else {
                 printf("Failed to load Greek font from: %s\n", greekFontPaths[i]);
            }
        }
    }
    
    // Unload codepoint data, no longer needed
    UnloadCodepoints(codepoints);
    
    if (!state.greekFontLoaded) {
        printf("Warning: Could not load any suitable Greek font. Greek text may not display correctly.\n");
        // Assign the original font to greekFont to avoid null issues later if needed, though it won't display correctly.
        state.greekFont = state.originalDefaultFont;
    }
    
    // Initialize localization system
    RayDialI18N* i18n = CreateI18NManager();
    state.i18n = i18n;
    
    // Add supported languages
    AddLanguage(i18n, "en", "English");
    AddLanguage(i18n, "es", "Español");
    AddLanguage(i18n, "el", "Ελληνικά"); // Greek
    
    // Set English as the default language
    SetCurrentLanguage(i18n, "en");
    
    // Define translation keys and values for English
    AddTranslation(i18n, "en", "title", "Localization Example");
    AddTranslation(i18n, "en", "greetingLabel", "Welcome to RayDial localization");
    AddTranslation(i18n, "en", "switchLanguage", "Switch Language");
    AddTranslation(i18n, "en", "dialogueTitle", "Sample Dialogue");
    AddTranslation(i18n, "en", "speakerName", "Guide");
    AddTranslation(i18n, "en", "dialogueText", "This is an example of [color=green]localized text[/color] with [size=large]styled[/size] formatting.");
    AddTranslation(i18n, "en", "currentLanguage", "Current Language: English");
    
    // Spanish translations
    AddTranslation(i18n, "es", "title", "Ejemplo de Localización");
    AddTranslation(i18n, "es", "greetingLabel", "Bienvenido a la localización de RayDial");
    AddTranslation(i18n, "es", "switchLanguage", "Cambiar Idioma");
    AddTranslation(i18n, "es", "dialogueTitle", "Diálogo de Ejemplo");
    AddTranslation(i18n, "es", "speakerName", "Guía");
    AddTranslation(i18n, "es", "dialogueText", "Este es un ejemplo de [color=green]texto localizado[/color] con formato [size=large]estilizado[/size].");
    AddTranslation(i18n, "es", "currentLanguage", "Idioma Actual: Español");
    
    // Greek translations
    AddTranslation(i18n, "el", "title", "Παράδειγμα τοπικοποίησης");
    AddTranslation(i18n, "el", "greetingLabel", "Καλώς ήρθατε στη τοπικοποίηση RayDial");
    AddTranslation(i18n, "el", "switchLanguage", "Αλλαγή γλώσσας");
    AddTranslation(i18n, "el", "dialogueTitle", "Δείγμα διαλόγου");
    AddTranslation(i18n, "el", "speakerName", "Οδηγός");
    AddTranslation(i18n, "el", "dialogueText", "Αυτό είναι ένα παράδειγμα [color=green]τοπικοποιημένου κειμένου[/color] με [size=large]μορφοποίηση[/size].");
    AddTranslation(i18n, "el", "currentLanguage", "Τρέχουσα γλώσσα: Ελληνικά");
    
    // Initialize application state
    state.currentLanguageIndex = 0;
    state.availableLanguages[0] = "en";
    state.availableLanguages[1] = "es";
    state.availableLanguages[2] = "el";
    
    // Create dialogue node
    RayDialNode* rootNode = CreateDialogueNode("root", "Localized Dialogue Node");
    
    // Create dialogue manager
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    // Create main UI panel
    RayDialComponent* panel = CreatePanel(
        (Rectangle){ 50, 50, 700, 500 },
        RAYWHITE
    );
    
    // Create title with localized text
    RayDialComponent* title = CreateLocalizedLabel(
        (Rectangle){ 70, 70, 660, 40 },
        "title",
        true,
        i18n
    );
    state.title = title;
    
    // Create greeting label with localized text
    RayDialComponent* greetingLabel = CreateLocalizedLabel(
        (Rectangle){ 70, 120, 660, 30 },
        "greetingLabel",
        true,
        i18n
    );
    state.greetingLabel = greetingLabel;
    
    // Create language display label
    RayDialComponent* languageLabel = CreateLocalizedLabel(
        (Rectangle){ 70, 160, 660, 30 },
        "currentLanguage",
        true,
        i18n
    );
    state.languageLabel = languageLabel;
    
    // Create portrait dialogue with localized text
    RayDialComponent* portraitDialogue = CreateLocalizedPortraitDialogue(
        (Rectangle){ 70, 200, 660, 220 },
        "speakerName",
        "dialogueText",
        BLUE,
        i18n
    );
    
    // Enable styled text for the dialogue
    SetLocalizedPortraitDialogueStyledText(portraitDialogue, "dialogueText", i18n);
    
    // Store reference to dialogue component for language switching
    state.dialogueComponent = portraitDialogue;
    
    // Create language switch button
    RayDialComponent* switchButton = CreateLocalizedButton(
        (Rectangle){ 300, 430, 200, 40 },
        "switchLanguage",
        OnSwitchLanguage,
        &state,
        i18n
    );
    state.switchButton = switchButton;
    
    // Add components to the panel
    AddComponent(panel, title);
    AddComponent(panel, greetingLabel);
    AddComponent(panel, languageLabel);
    AddComponent(panel, portraitDialogue);
    AddComponent(panel, switchButton);
    
    // Set the panel as the root node's component
    rootNode->components = panel;
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateDialogueManager(manager);

        // Determine current font based on language
        Font currentFont = (state.currentLanguageIndex == 2 && state.greekFontLoaded) ? state.greekFont : state.originalDefaultFont;

        // Draw
        BeginDrawing();
            ClearBackground(SKYBLUE);

            // --- Manual Drawing of Components --- 
            // We bypass DrawDialogueManager to control font usage

            // 1. Draw Panel Background (using data from the panel component)
            RayDialPanelData* panelData = (RayDialPanelData*)panel->data;
            DrawRectangleRec(panel->bounds, panelData->backgroundColor);
            DrawRectangleLinesEx(panel->bounds, panelData->borderWidth, panelData->borderColor);

            // Get localized text for components manually
            const char* titleText = GetLocalizedText(state.i18n, "title");
            const char* greetingText = GetLocalizedText(state.i18n, "greetingLabel");
            const char* langLabelText = GetLocalizedText(state.i18n, "currentLanguage");
            const char* buttonText = GetLocalizedText(state.i18n, "switchLanguage");
            const char* speakerNameText = GetLocalizedText(state.i18n, "speakerName");
            // Note: Dialogue text needs special handling for styled text

            // 2. Draw Title Label (using raylib directly)
            RayDialLabelData* titleData = (RayDialLabelData*)state.title->data;
            DrawTextEx(currentFont, titleText, 
                       (Vector2){state.title->bounds.x, state.title->bounds.y}, 
                       titleData->fontSize, 1.0f, titleData->textColor);

            // 3. Draw Greeting Label
            RayDialLabelData* greetingData = (RayDialLabelData*)state.greetingLabel->data;
            DrawTextEx(currentFont, greetingText, 
                       (Vector2){state.greetingLabel->bounds.x, state.greetingLabel->bounds.y}, 
                       greetingData->fontSize, 1.0f, greetingData->textColor);

            // 4. Draw Current Language Label
            RayDialLabelData* langLabelData = (RayDialLabelData*)state.languageLabel->data;
            DrawTextEx(currentFont, langLabelText, 
                       (Vector2){state.languageLabel->bounds.x, state.languageLabel->bounds.y}, 
                       langLabelData->fontSize, 1.0f, langLabelData->textColor);

            // 5. Draw Portrait Dialogue (More complex - replicating parts of DrawComponent)
            RayDialPortraitDialogueData* dialogueData = (RayDialPortraitDialogueData*)state.dialogueComponent->data;
            Rectangle dialogueBounds = state.dialogueComponent->bounds;
            int portraitSize = dialogueData->portraitSize;
            int padding = 10;
            int nameHeight = dialogueData->speakerName ? 40 : 0;
            float portraitX = dialogueData->showOnRight ? 
                (dialogueBounds.x + dialogueBounds.width - portraitSize - padding) : 
                (dialogueBounds.x + padding);
            float portraitY = dialogueBounds.y + padding;
            
            // Draw dialogue box background
            DrawRectangleRec(dialogueBounds, dialogueData->dialogueBoxColor);
            DrawRectangleLinesEx(dialogueBounds, 2, DARKGRAY);

            // Draw portrait (color only for simplicity here)
            Rectangle portraitRect = { portraitX, portraitY, portraitSize, portraitSize };
             if (dialogueData->useTexture) { // Added texture support back
                 DrawTexturePro(
                     dialogueData->portraitTexture,
                     (Rectangle){ 0, 0, dialogueData->portraitTexture.width, dialogueData->portraitTexture.height },
                     portraitRect,
                     (Vector2){ 0, 0 },
                     0.0f,
                     WHITE
                 );
             } else {
                 DrawRectangleRec(portraitRect, dialogueData->portraitColor);
                 DrawRectangleLinesEx(portraitRect, 2, DARKGRAY);
             }

            // Draw name tag
            if (speakerNameText) {
                Rectangle nameTagRect;
                if (dialogueData->showOnRight) { nameTagRect = (Rectangle){ portraitX - 120, portraitY, 120, nameHeight }; }
                else { nameTagRect = (Rectangle){ portraitX + portraitSize, portraitY, 120, nameHeight }; }
                DrawRectangleRec(nameTagRect, dialogueData->nameTagColor);
                DrawRectangleLinesEx(nameTagRect, 2, DARKGRAY);
                Vector2 nameSize = MeasureTextEx(currentFont, speakerNameText, dialogueData->nameFontSize, 1.0f);
                float nameX = nameTagRect.x + (nameTagRect.width - nameSize.x) / 2;
                float nameY = nameTagRect.y + (nameTagRect.height - dialogueData->nameFontSize) / 2;
                DrawTextEx(currentFont, speakerNameText, (Vector2){nameX, nameY}, dialogueData->nameFontSize, 1.0f, dialogueData->nameColor);
            }

            // Draw dialogue text (handling styled vs plain)
            float textAreaX = dialogueData->showOnRight ? dialogueBounds.x + padding : portraitX + portraitSize + padding;
            float textAreaWidth = dialogueData->showOnRight ? 
                (portraitX - dialogueBounds.x - padding * 2) : 
                (dialogueBounds.x + dialogueBounds.width - textAreaX - padding);
            float textAreaY = portraitY + nameHeight + padding;
            float textAreaHeight = dialogueBounds.height - nameHeight - padding * 3;
            Rectangle textArea = { textAreaX, textAreaY, textAreaWidth, textAreaHeight };

            BeginScissorMode(textArea.x, textArea.y, textArea.width, textArea.height);
            if (dialogueData->useStyledText && dialogueData->styledText) {
                // --- Draw Styled Text Segments --- 
                float currentX = textArea.x;
                float currentY = textArea.y;
                float baseFontSize = (float)dialogueData->fontSize; // Use dialogue's base font size
                float lineHeight = baseFontSize * 1.5f; // Base line height
                float spaceWidth = MeasureTextEx(currentFont, " ", baseFontSize, 1.0f).x; // Width of a space

                RayDialTextSegment* segment = dialogueData->styledText;
                while (segment) {
                    // Determine style for this segment
                    Color segmentColor = dialogueData->textColor; // Default
                    float segmentFontSize = baseFontSize;        // Default
                    // TODO: Add bold/italic handling if needed (requires font variants)

                    RayDialTextStyle* style = segment->styles;
                    while (style) {
                        if (style->type == RAYDIAL_TEXT_COLORED) {
                            segmentColor = style->value.color;
                        } else if (style->type == RAYDIAL_TEXT_SIZED) {
                            segmentFontSize = style->value.fontSize;
                        }
                        // Add cases for bold/italic here if implemented
                        style = style->next;
                    }

                    // --- Basic Word Wrapping for Segment --- 
                    #define MAX_WORD_BUFFER 128 // Max chars for a single word buffer
                    char wordBuffer[MAX_WORD_BUFFER];
                    const char* textPtr = segment->text;
                    while (*textPtr) {
                        // Find the next word or end of segment
                        const char* wordStart = textPtr;
                        const char* wordEnd = wordStart;
                        while (*wordEnd && *wordEnd != ' ' && *wordEnd != '\n') {
                            wordEnd++;
                        }
                        // Include trailing space if not at the end of the segment string
                        if (*wordEnd == ' ') wordEnd++; 
                        
                        int wordLen = wordEnd - wordStart;
                        if (wordLen <= 0) break; 
                        if (wordLen >= MAX_WORD_BUFFER) wordLen = MAX_WORD_BUFFER - 1; // Prevent buffer overflow

                        // Measure the word with its style
                        // Copy word to buffer and measure
                        strncpy(wordBuffer, wordStart, wordLen);
                        wordBuffer[wordLen] = '\0'; // Null-terminate
                        Vector2 wordSize = MeasureTextEx(currentFont, wordBuffer, segmentFontSize, 1.0f);

                        // Check if word fits on the current line
                        if (currentX > textArea.x && currentX + wordSize.x > textArea.x + textArea.width) {
                            // Doesn't fit, move to the next line
                            currentX = textArea.x;
                            currentY += lineHeight; // Advance line
                             // Check if we've gone past the vertical bounds
                             if (currentY >= textArea.y + textArea.height) break; // Stop drawing text
                        }

                        // Draw the word if within vertical bounds
                        if (currentY < textArea.y + textArea.height) {
                            // Draw word from buffer
                            DrawTextEx(currentFont, wordBuffer, 
                                       (Vector2){currentX, currentY}, 
                                       segmentFontSize, 1.0f, segmentColor);
                        }

                        currentX += wordSize.x; // Advance position
                        textPtr = wordEnd; // Move pointer to start of next word
                    }
                     if (currentY >= textArea.y + textArea.height) break; // Stop drawing segments

                    segment = segment->next; // Next segment
                }
            } else {
                const char* dialogueTextPlain = GetLocalizedText(state.i18n, "dialogueText");
                DrawTextEx(currentFont, dialogueTextPlain ? dialogueTextPlain : "", 
                           (Vector2){textArea.x, textArea.y}, 
                           dialogueData->fontSize, 1.0f, dialogueData->textColor);
            }
            EndScissorMode();

            // 6. Draw Button
            RayDialButtonData* buttonData = (RayDialButtonData*)state.switchButton->data;
            bool isHovered = CheckCollisionPointRec(GetMousePosition(), state.switchButton->bounds);
            DrawRectangleRec(state.switchButton->bounds, isHovered ? buttonData->hoverColor : buttonData->backgroundColor);
            Vector2 buttonTextSize = MeasureTextEx(currentFont, buttonText, buttonData->fontSize, 1.0f);
            float buttonTextX = state.switchButton->bounds.x + (state.switchButton->bounds.width - buttonTextSize.x) / 2;
            float buttonTextY = state.switchButton->bounds.y + (state.switchButton->bounds.height - buttonData->fontSize) / 2;
            DrawTextEx(currentFont, buttonText, (Vector2){buttonTextX, buttonTextY}, buttonData->fontSize, 1.0f, buttonData->textColor);

            // --- End Manual Drawing --- 

            // Display current language in the status bar (using currentFont)
            if (state.currentLanguageIndex == 2) { // Greek selected
                DrawRectangle(50, 10, 700, 30, DARKGRAY);
                 // The default font is already set to greekFont if loaded, so DrawText works - COMMENT IS WRONG
                // DrawText("Currently using Greek (Ελληνικά)", 60, 15, 20, WHITE); // <-- Uses default font
                DrawTextEx(currentFont, "Currently using Greek (Ελληνικά)", (Vector2){60, 15}, 20, 1.0f, WHITE); // <-- Use currentFont
            } else {
                // Optionally display for other languages if needed
                 // DrawRectangle(50, 10, 700, 30, DARKGRAY);
                 // DrawText(TextFormat("Currently using %s", GetCurrentLanguageName(state.i18n)), 60, 15, 20, WHITE);
            }
            
        EndDrawing();
    }
    
    // Cleanup
    if (state.greekFontLoaded && state.greekFont.texture.id != state.originalDefaultFont.texture.id) {
        UnloadFont(state.greekFont); // Unload only if it's different from original
        printf("Unloaded Greek font.\n");
    }

    FreeDialogueManager(manager);
    FreeI18NManager(i18n);
    
    CloseWindow();
    return 0;
}

// Callback function for language switching
void OnSwitchLanguage(void* userData) {
    AppState* state = (AppState*)userData;
    
    // Cycle to the next language
    state->currentLanguageIndex = (state->currentLanguageIndex + 1) % 3;
    const char* newLanguage = state->availableLanguages[state->currentLanguageIndex];
    
    // Apply the new language to the I18N manager
    if (SetCurrentLanguage(state->i18n, newLanguage)) {
        printf("Switched to language: %s\n", GetCurrentLanguageName(state->i18n));
        
        // Update component text keys - the drawing loop now handles font selection
        SetLocalizedLabelText(state->title, "title", state->i18n);
        SetLocalizedLabelText(state->greetingLabel, "greetingLabel", state->i18n);
        SetLocalizedLabelText(state->languageLabel, "currentLanguage", state->i18n);
        SetLocalizedButtonText(state->switchButton, "switchLanguage", state->i18n);
        
        // Update the dialogue component's keys - IMPORTANT for styled text
        SetLocalizedPortraitDialogueSpeaker(state->dialogueComponent, "speakerName", state->i18n);
        // Re-parse styled text when language changes to ensure correct segments are generated
        SetLocalizedPortraitDialogueStyledText(state->dialogueComponent, "dialogueText", state->i18n);
        
        // No need to change default font anymore
    }
} 