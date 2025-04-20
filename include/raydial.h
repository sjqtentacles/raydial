#ifndef RAYDIAL_H
#define RAYDIAL_H

#include <raylib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Callback function type for UI interactions
typedef void (*RayDialCallback)(void* userData);

// UI Component types
typedef enum {
    RAYDIAL_BUTTON,
    RAYDIAL_LABEL,
    RAYDIAL_TEXTBOX,
    RAYDIAL_IMAGE,
    RAYDIAL_PANEL,
    RAYDIAL_SCROLLAREA,
    RAYDIAL_PORTRAIT_DIALOGUE  // New component for portrait dialogue
} RayDialComponentType;

// Text style types for rich text rendering
typedef enum {
    RAYDIAL_TEXT_REGULAR,
    RAYDIAL_TEXT_COLORED,
    RAYDIAL_TEXT_SIZED,
    RAYDIAL_TEXT_BOLD,
    RAYDIAL_TEXT_ITALIC
} RayDialTextStyleType;

// Text style element for rich text rendering
typedef struct RayDialTextStyle {
    RayDialTextStyleType type;
    union {
        Color color;              // For RAYDIAL_TEXT_COLORED
        float fontSize;           // For RAYDIAL_TEXT_SIZED
    } value;
    struct RayDialTextStyle* next;  // Linked list of styles
} RayDialTextStyle;

// Styled text segment for rich text rendering
typedef struct RayDialTextSegment {
    char* text;                      // Text content
    RayDialTextStyle* styles;        // Linked list of styles
    struct RayDialTextSegment* next; // Next segment in the text
} RayDialTextSegment;

// Base UI component structure
typedef struct RayDialComponent {
    RayDialComponentType type;
    Rectangle bounds;
    bool visible;
    bool enabled;
    void* data;  // Component-specific data
    RayDialCallback onClick;
    void* userData;
    struct RayDialComponent* next;  // Linked list for child components
} RayDialComponent;

// Button specific data
typedef struct {
    const char* text;
    Color textColor;
    Color backgroundColor;
    Color hoverColor;
    int fontSize;
} RayDialButtonData;

// Label specific data
typedef struct {
    const char* text;
    Color textColor;
    int fontSize;
    bool wrapText;
    // Scrolling properties
    float scrollPosition;
    float contentHeight;
    bool scrollable;
    Color scrollbarColor;
    int scrollbarWidth;
} RayDialLabelData;

// Textbox specific data
typedef struct {
    char* text;
    int maxLength;
    Color textColor;
    Color backgroundColor;
    int fontSize;
    bool isPassword;
} RayDialTextboxData;

// Panel specific data
typedef struct {
    Color backgroundColor;
    Color borderColor;
    int borderWidth;
    int padding;
} RayDialPanelData;

// Scroll area specific data
typedef struct {
    float scrollPosition;
    float contentHeight;
    Color scrollbarColor;
    int scrollbarWidth;
} RayDialScrollAreaData;

// Portrait dialogue specific data
typedef struct {
    const char* speakerName;          // Name of the character speaking
    const char* dialogueText;         // The dialogue text (plain text version)
    RayDialTextSegment* styledText;   // Styled segments of text (if using rich text)
    bool useStyledText;               // Whether to use rich text rendering
    Color portraitColor;              // Color to use for portrait (if no texture)
    Texture2D portraitTexture;        // Portrait texture (optional, uses color if no texture)
    bool useTexture;                  // Whether to use texture or color
    Color nameTagColor;               // Background color for name tag
    Color dialogueBoxColor;           // Background color for dialogue box
    Color textColor;                  // Default color for dialogue text
    Color nameColor;                  // Color for name text
    int fontSize;                     // Default font size for dialogue text
    int nameFontSize;                 // Font size for name text
    bool wrapText;                    // Whether to wrap dialogue text
    int portraitSize;                 // Size of the portrait (square)
    bool showOnRight;                 // Whether to show portrait on right (default: left)
} RayDialPortraitDialogueData;

// Dialogue node structure for dialogue trees
typedef struct RayDialNode {
    const char* id;
    const char* text;
    RayDialComponent* components;
    struct RayDialNode** choices;  // Array of choice nodes
    int choiceCount;
    RayDialCallback onEnter;
    RayDialCallback onExit;
    void* userData;
} RayDialNode;

// Dialogue manager structure
typedef struct {
    RayDialNode* currentNode;
    RayDialNode* rootNode;
    bool isActive;
    void* userData;
} RayDialManager;

// Function declarations for UI components
RayDialComponent* CreateButton(Rectangle bounds, const char* text, RayDialCallback onClick, void* userData);
RayDialComponent* CreateLabel(Rectangle bounds, const char* text, bool wrapText);
RayDialComponent* CreateTextbox(Rectangle bounds, int maxLength);
RayDialComponent* CreatePanel(Rectangle bounds, Color backgroundColor);
RayDialComponent* CreateScrollArea(Rectangle bounds, float contentHeight);
RayDialComponent* CreatePortraitDialogue(Rectangle bounds, const char* speakerName, const char* dialogueText, Color portraitColor);
RayDialComponent* CreatePortraitDialogueWithTexture(Rectangle bounds, const char* speakerName, const char* dialogueText, Texture2D portraitTexture);

// Function declarations for component management
void AddComponent(RayDialComponent* parent, RayDialComponent* child);
void RemoveComponent(RayDialComponent* parent, RayDialComponent* child);
void UpdateComponent(RayDialComponent* component);
void DrawComponent(RayDialComponent* component);
void FreeComponent(RayDialComponent* component);

// Function declarations for dialogue trees
RayDialNode* CreateDialogueNode(const char* id, const char* text);
void AddChoice(RayDialNode* node, RayDialNode* choice);
void SetNodeCallbacks(RayDialNode* node, RayDialCallback onEnter, RayDialCallback onExit, void* userData);

// Function declarations for dialogue manager
RayDialManager* CreateDialogueManager(RayDialNode* rootNode);
void UpdateDialogueManager(RayDialManager* manager);
void DrawDialogueManager(RayDialManager* manager);
void FreeDialogueManager(RayDialManager* manager);
void TransitionToNode(RayDialManager* manager, const char* nodeId);

// Utility functions
bool IsComponentClicked(RayDialComponent* component);
bool IsComponentHovered(RayDialComponent* component);
void SetComponentEnabled(RayDialComponent* component, bool enabled);
void SetComponentVisible(RayDialComponent* component, bool visible);

// Portrait dialogue utility functions
void SetPortraitDialogueText(RayDialComponent* component, const char* dialogueText);
void SetPortraitDialogueSpeaker(RayDialComponent* component, const char* speakerName);
void SetPortraitDialogueColor(RayDialComponent* component, Color portraitColor);
void SetPortraitDialogueTexture(RayDialComponent* component, Texture2D portraitTexture);
void SetPortraitDialoguePosition(RayDialComponent* component, bool showOnRight);

// Rich text utility functions
void SetPortraitDialogueStyledText(RayDialComponent* component, const char* formattedText);
RayDialTextSegment* ParseStyledText(const char* formattedText, Color defaultColor, float defaultFontSize);
void FreeStyledText(RayDialTextSegment* styledText);
Color GetColorFromName(const char* colorName);

#ifdef __cplusplus
}
#endif

#endif // RAYDIAL_H