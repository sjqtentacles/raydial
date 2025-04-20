# RayDial API Reference

This document provides a comprehensive guide to using the RayDial API.

## Core Components

### Component Types

RayDial uses a component-based architecture with the following component types:

```c
typedef enum {
    RAYDIAL_BUTTON,    // Interactive buttons
    RAYDIAL_LABEL,     // Text labels with wrapping and scrolling
    RAYDIAL_TEXTBOX,   // Input fields (future)
    RAYDIAL_IMAGE,     // Image display (future)
    RAYDIAL_PANEL,     // Container for other components
    RAYDIAL_SCROLLAREA // Scrollable area (future)
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

## Utility Functions

### Component State

```c
bool IsComponentClicked(RayDialComponent* component);
bool IsComponentHovered(RayDialComponent* component);
void SetComponentEnabled(RayDialComponent* component, bool enabled);
void SetComponentVisible(RayDialComponent* component, bool visible);
```

## Example Usage

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