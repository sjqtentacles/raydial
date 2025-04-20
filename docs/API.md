# RayDial API Reference

This document provides a comprehensive guide to using the RayDial API.

## Core Components

### Component Types

RayDial uses a component-based architecture with the following component types:

```c
typedef enum {
    RAYDIAL_BUTTON,           // Interactive buttons
    RAYDIAL_LABEL,            // Text labels with wrapping and scrolling
    RAYDIAL_TEXTBOX,          // Input fields (future)
    RAYDIAL_IMAGE,            // Image display (future)
    RAYDIAL_PANEL,            // Container for other components
    RAYDIAL_SCROLLAREA,       // Scrollable area (future)
    RAYDIAL_PORTRAIT_DIALOGUE // Character dialogue with portrait
} RayDialComponentType;
```

### Base Component Structure

All UI components share this common structure:

```c
typedef struct RayDialComponent {
    RayDialComponentType type;
    Rectangle bounds;
    bool visible;
    bool enabled;
    void* data;                    // Component-specific data
    RayDialCallback onClick;
    void* userData;
    struct RayDialComponent* next; // Linked list for child components
} RayDialComponent;
```

## Component Creation

### Button

```c
RayDialComponent* CreateButton(
    Rectangle bounds,           // Position and size
    const char* text,           // Button text
    RayDialCallback onClick,    // Callback function
    void* userData              // User data passed to callback
);
```

### Label

```c
RayDialComponent* CreateLabel(
    Rectangle bounds,           // Position and size
    const char* text,           // Label text
    bool wrapText               // Enable word wrapping
);
```

### Panel

```c
RayDialComponent* CreatePanel(
    Rectangle bounds,           // Position and size
    Color backgroundColor       // Background color
);
```

### Portrait Dialogue

```c
RayDialComponent* CreatePortraitDialogue(
    Rectangle bounds,           // Position and size
    const char* speakerName,    // Name of the character speaking
    const char* dialogueText,   // Plain text for dialogue
    Color portraitColor         // Color for portrait background
);
```

```c
RayDialComponent* CreatePortraitDialogueWithTexture(
    Rectangle bounds,           // Position and size
    const char* speakerName,    // Name of the character speaking
    const char* dialogueText,   // Plain text for dialogue
    Texture2D portraitTexture   // Texture for character portrait
);
```

## Component Management

### Adding Child Components

```c
void AddComponent(
    RayDialComponent* parent,   // Parent component
    RayDialComponent* child     // Child component to add
);
```

### Removing Child Components

```c
void RemoveComponent(
    RayDialComponent* parent,   // Parent component
    RayDialComponent* child     // Child component to remove
);
```

### Free Components

```c
void FreeComponent(
    RayDialComponent* component // Component to free
);
```

## Dialogue System

### Dialogue Node

Dialogue nodes form the structure of dialogue trees:

```c
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
```

### Creating Dialogue Nodes

```c
RayDialNode* CreateDialogueNode(
    const char* id,             // Unique identifier
    const char* text            // Node description/text
);
```

### Adding Choices

```c
void AddChoice(
    RayDialNode* node,          // Parent node
    RayDialNode* choice         // Child node (choice)
);
```

### Setting Node Callbacks

```c
void SetNodeCallbacks(
    RayDialNode* node,          // Target node
    RayDialCallback onEnter,    // Called when entering node
    RayDialCallback onExit,     // Called when exiting node
    void* userData              // User data for callbacks
);
```

## Dialogue Manager

The dialogue manager handles the active dialogue tree:

```c
typedef struct {
    RayDialNode* currentNode;
    RayDialNode* rootNode;
    bool isActive;
    void* userData;
} RayDialManager;
```

### Creating a Dialogue Manager

```c
RayDialManager* CreateDialogueManager(
    RayDialNode* rootNode       // Root dialogue node
);
```

### Updating and Drawing

```c
void UpdateDialogueManager(
    RayDialManager* manager     // Dialogue manager to update
);

void DrawDialogueManager(
    RayDialManager* manager     // Dialogue manager to draw
);
```

### Node Transitions

```c
void TransitionToNode(
    RayDialManager* manager,    // Dialogue manager
    const char* nodeId          // Target node ID
);
```

### Cleanup

```c
void FreeDialogueManager(
    RayDialManager* manager     // Dialogue manager to free
);
```

## Portrait Dialogue Utilities

Functions for manipulating portrait dialogue components:

```c
// Set dialogue text (plain text)
void SetPortraitDialogueText(
    RayDialComponent* component, 
    const char* dialogueText
);

// Set speaker name
void SetPortraitDialogueSpeaker(
    RayDialComponent* component, 
    const char* speakerName
);

// Set portrait color (disables texture)
void SetPortraitDialogueColor(
    RayDialComponent* component, 
    Color portraitColor
);

// Set portrait texture
void SetPortraitDialogueTexture(
    RayDialComponent* component, 
    Texture2D portraitTexture
);

// Set portrait position (left or right)
void SetPortraitDialoguePosition(
    RayDialComponent* component, 
    bool showOnRight
);
```

## Styled Text System

RayDial includes a rich text system that allows formatting text with styles like colors, sizes, bold, and italic.

### Styled Text Data Structures

```c
// Text style types
typedef enum {
    RAYDIAL_TEXT_REGULAR,
    RAYDIAL_TEXT_COLORED,
    RAYDIAL_TEXT_SIZED,
    RAYDIAL_TEXT_BOLD,
    RAYDIAL_TEXT_ITALIC
} RayDialTextStyleType;

// Text style element
typedef struct RayDialTextStyle {
    RayDialTextStyleType type;
    union {
        Color color;              // For RAYDIAL_TEXT_COLORED
        float fontSize;           // For RAYDIAL_TEXT_SIZED
    } value;
    struct RayDialTextStyle* next;  // Linked list of styles
} RayDialTextStyle;

// Styled text segment
typedef struct RayDialTextSegment {
    char* text;                      // Text content
    RayDialTextStyle* styles;        // Linked list of styles
    struct RayDialTextSegment* next; // Next segment in the text
} RayDialTextSegment;
```

### Styled Text Functions

```c
// Set styled text for a portrait dialogue
void SetPortraitDialogueStyledText(
    RayDialComponent* component,  // Portrait dialogue component
    const char* formattedText     // Text with formatting tags
);

// Parse formatted text into styled text segments
RayDialTextSegment* ParseStyledText(
    const char* formattedText,    // Text with formatting tags
    Color defaultColor,           // Default text color
    float defaultFontSize         // Default font size
);

// Free styled text segments
void FreeStyledText(
    RayDialTextSegment* styledText
);

// Get color from name (used by the parser)
Color GetColorFromName(
    const char* colorName         // Color name (e.g., "red", "blue")
);
```

### Supported Tags

The styled text system supports the following tags:

| Tag | Description | Example |
|-----|-------------|---------|
| `[color=NAME]` | Changes text color | `[color=red]Red text[/color]` |
| `[size=VALUE]` | Changes font size | `[size=24]Large text[/size]` |
| `[b]` | Bold text | `[b]Bold text[/b]` |
| `[i]` | Italic text | `[i]Italic text[/i]` |

### Named Colors

The following color names are supported:

- `red`
- `green`
- `blue`
- `yellow`
- `purple`
- `orange`
- `white`
- `black`
- `gray`/`grey`
- `darkgray`
- `lightgray`

### Size Values

For size tags, you can use:

- Numeric values: `[size=24]`
- Named values: `[size=small]`, `[size=large]`, `[size=huge]`

### Nested Tags

Tags can be nested to combine styles:

```
[color=red][b]Bold red text[/b][/color]
[size=large][color=blue]Large blue text[/color][/size]
```

## Example Usage: Styled Text

```c
// Create a portrait dialogue with styled text
RayDialComponent* dialogue = CreatePortraitDialogue(
    (Rectangle){50, 50, 700, 200},
    "Character",
    NULL,  // No plain text, using styled text instead
    BLUE
);

// Set styled text with various formats
SetPortraitDialogueStyledText(
    dialogue,
    "This is [color=red]red text[/color] and this is [b]bold text[/b].\n"
    "This is [size=large]larger text[/size] and this is [i]italic text[/i].\n"
    "You can also [color=green][b]combine[/b][/color] [size=24][i]styles[/i][/size]!"
);
```

## Utility Functions

### Component State

```c
bool IsComponentClicked(RayDialComponent* component);
bool IsComponentHovered(RayDialComponent* component);
void SetComponentEnabled(RayDialComponent* component, bool enabled);
void SetComponentVisible(RayDialComponent* component, bool visible);
```

## Example Usage: Dialogue System

Here's a complete example of a simple dialogue with choices:

```c
// Create nodes
RayDialNode* rootNode = CreateDialogueNode("root", "Hello, traveler!");
RayDialNode* friendlyNode = CreateDialogueNode("friendly", "Good to meet you!");
RayDialNode* hostileNode = CreateDialogueNode("hostile", "What do you want?");

// Add choices to root node
AddChoice(rootNode, friendlyNode);
AddChoice(rootNode, hostileNode);

// Create dialogue manager
RayDialManager* manager = CreateDialogueManager(rootNode);

// Create UI for root node
RayDialComponent* panel = CreatePanel((Rectangle){100, 100, 600, 400}, RAYWHITE);
RayDialComponent* label = CreateLabel((Rectangle){120, 120, 560, 100}, rootNode->text, true);
RayDialComponent* button1 = CreateButton((Rectangle){120, 250, 250, 50}, "Be Friendly", OnFriendlyClicked, manager);
RayDialComponent* button2 = CreateButton((Rectangle){400, 250, 250, 50}, "Be Hostile", OnHostileClicked, manager);

// Assemble UI
AddComponent(panel, label);
AddComponent(panel, button1);
AddComponent(panel, button2);
rootNode->components = panel;

// Callback implementations
void OnFriendlyClicked(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "friendly");
}

void OnHostileClicked(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "hostile");
}
``` 