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
    RAYDIAL_SCROLLAREA
} RayDialComponentType;

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

#ifdef __cplusplus
}
#endif

#endif // RAYDIAL_H