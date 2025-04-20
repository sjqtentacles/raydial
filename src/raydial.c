#include "raydial.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// Component creation functions
RayDialComponent* CreateButton(Rectangle bounds, const char* text, RayDialCallback onClick, void* userData) {
    RayDialComponent* component = (RayDialComponent*)malloc(sizeof(RayDialComponent));
    RayDialButtonData* data = (RayDialButtonData*)malloc(sizeof(RayDialButtonData));
    
    component->type = RAYDIAL_BUTTON;
    component->bounds = bounds;
    component->visible = true;
    component->enabled = true;
    component->data = data;
    component->onClick = onClick;
    component->userData = userData;
    component->next = NULL;
    
    data->text = text;
    data->textColor = BLACK;
    data->backgroundColor = LIGHTGRAY;
    data->hoverColor = GRAY;
    data->fontSize = 20;
    
    return component;
}

RayDialComponent* CreateLabel(Rectangle bounds, const char* text, bool wrapText) {
    RayDialComponent* component = (RayDialComponent*)malloc(sizeof(RayDialComponent));
    RayDialLabelData* data = (RayDialLabelData*)malloc(sizeof(RayDialLabelData));
    
    component->type = RAYDIAL_LABEL;
    component->bounds = bounds;
    component->visible = true;
    component->enabled = true;
    component->data = data;
    component->onClick = NULL;
    component->userData = NULL;
    component->next = NULL;
    
    data->text = text;
    data->textColor = BLACK;
    data->fontSize = 20;
    data->wrapText = wrapText;
    
    // Initialize scrolling properties
    data->scrollPosition = 0.0f;
    data->contentHeight = 0.0f;  // Will be calculated during drawing
    data->scrollable = true;
    data->scrollbarColor = GRAY;
    data->scrollbarWidth = 8;
    
    return component;
}

RayDialComponent* CreatePanel(Rectangle bounds, Color backgroundColor) {
    RayDialComponent* component = (RayDialComponent*)malloc(sizeof(RayDialComponent));
    RayDialPanelData* data = (RayDialPanelData*)malloc(sizeof(RayDialPanelData));
    
    component->type = RAYDIAL_PANEL;
    component->bounds = bounds;
    component->visible = true;
    component->enabled = true;
    component->data = data;
    component->onClick = NULL;
    component->userData = NULL;
    component->next = NULL;
    
    data->backgroundColor = backgroundColor;
    data->borderColor = DARKGRAY;
    data->borderWidth = 2;
    data->padding = 10;
    
    return component;
}

// Component management functions
void AddComponent(RayDialComponent* parent, RayDialComponent* child) {
    if (!parent || !child) return;
    
    RayDialComponent* current = parent;
    while (current->next) {
        current = current->next;
    }
    current->next = child;
}

void RemoveComponent(RayDialComponent* parent, RayDialComponent* child) {
    if (!parent || !child) return;
    
    RayDialComponent* current = parent;
    while (current->next && current->next != child) {
        current = current->next;
    }
    
    if (current->next == child) {
        current->next = child->next;
        child->next = NULL;
    }
}

void UpdateComponent(RayDialComponent* component) {
    if (!component || !component->visible || !component->enabled) return;
    
    switch (component->type) {
        case RAYDIAL_BUTTON: {
            // Only trigger onClick if component is actually clicked this frame
            if (IsComponentClicked(component) && component->onClick) {
                component->onClick(component->userData);
            }
            break;
        }
        case RAYDIAL_LABEL: {
            RayDialLabelData* data = (RayDialLabelData*)component->data;
            
            // Handle keyboard scrolling if this component is hovered
            if (data->scrollable && data->contentHeight > 0 && IsComponentHovered(component)) {
                float scrollSpeed = 10.0f;
                
                // Up/Down arrow keys for scrolling
                if (IsKeyDown(KEY_UP)) {
                    data->scrollPosition -= scrollSpeed;
                } else if (IsKeyDown(KEY_DOWN)) {
                    data->scrollPosition += scrollSpeed;
                } else if (IsKeyDown(KEY_PAGE_UP)) {
                    data->scrollPosition -= component->bounds.height / 2;
                } else if (IsKeyDown(KEY_PAGE_DOWN)) {
                    data->scrollPosition += component->bounds.height / 2;
                }
                
                // Clamp scroll position
                if (data->scrollPosition < 0) {
                    data->scrollPosition = 0;
                } else if (data->scrollPosition > fmaxf(0, data->contentHeight - component->bounds.height)) {
                    data->scrollPosition = fmaxf(0, data->contentHeight - component->bounds.height);
                }
            }
            break;
        }
        case RAYDIAL_TEXTBOX: {
            // Handle text input
            break;
        }
        default:
            break;
    }
    
    // Update child components
    RayDialComponent* child = component->next;
    while (child) {
        UpdateComponent(child);
        child = child->next;
    }
}

void DrawComponent(RayDialComponent* component) {
    if (!component || !component->visible) return;
    
    switch (component->type) {
        case RAYDIAL_BUTTON: {
            RayDialButtonData* data = (RayDialButtonData*)component->data;
            bool isHovered = IsComponentHovered(component);
            
            // Draw button background
            DrawRectangleRec(component->bounds, 
                isHovered ? data->hoverColor : data->backgroundColor);
            
            // Draw button text
            int textWidth = MeasureText(data->text, data->fontSize);
            int textX = component->bounds.x + (component->bounds.width - textWidth) / 2;
            int textY = component->bounds.y + (component->bounds.height - data->fontSize) / 2;
            DrawText(data->text, textX, textY, data->fontSize, data->textColor);
            break;
        }
        case RAYDIAL_LABEL: {
            RayDialLabelData* data = (RayDialLabelData*)component->data;
            
            // Define scissor rectangle for clipping text to bounds
            Rectangle scissorRect = component->bounds;
            BeginScissorMode(scissorRect.x, scissorRect.y, scissorRect.width, scissorRect.height);
            
            // Get text properties
            const char* text = data->text;
            int fontSize = data->fontSize;
            float lineHeight = fontSize * 1.5f;
            
            // Draw text based on wrapping setting
            if (data->wrapText) {
                // Create a working copy of the text
                size_t textLen = strlen(text);
                char* textCopy = (char*)malloc(textLen + 1);
                if (!textCopy) {
                    EndScissorMode();
                    break;
                }
                strcpy(textCopy, text);
                
                // Word wrapping implementation
                float x = component->bounds.x;
                float y = component->bounds.y - data->scrollPosition;
                float maxWidth = component->bounds.width - (data->scrollable ? data->scrollbarWidth + 5 : 0);
                
                // Reset content height for calculation
                data->contentHeight = 0;
                
                char* token = strtok(textCopy, " \n");
                char line[1024] = {0};
                
                while (token) {
                    // Handle explicit newlines in token
                    if (strchr(token, '\n')) {
                        char* newlinePtr = strchr(token, '\n');
                        *newlinePtr = '\0';
                        
                        // Add current token to line
                        char tempLine[1024];
                        if (strlen(line) > 0) {
                            sprintf(tempLine, "%s %s", line, token);
                        } else {
                            sprintf(tempLine, "%s", token);
                        }
                        
                        // Draw current line
                        if (y + lineHeight > component->bounds.y && y < component->bounds.y + component->bounds.height) {
                            DrawText(line, x, y, fontSize, data->textColor);
                        }
                        
                        // Move to next line
                        y += lineHeight;
                        data->contentHeight += lineHeight;
                        line[0] = '\0';
                        
                        // Process remainder after newline
                        token = newlinePtr + 1;
                        if (!*token) {
                            token = strtok(NULL, " \n");
                        }
                        continue;
                    }
                    
                    // Check if adding this token exceeds the max width
                    char testLine[1024];
                    if (strlen(line) > 0) {
                        sprintf(testLine, "%s %s", line, token);
                    } else {
                        sprintf(testLine, "%s", token);
                    }
                    
                    int textWidth = MeasureText(testLine, fontSize);
                    
                    if (textWidth > maxWidth && strlen(line) > 0) {
                        // Draw current line and move to next line
                        if (y + lineHeight > component->bounds.y && y < component->bounds.y + component->bounds.height) {
                            DrawText(line, x, y, fontSize, data->textColor);
                        }
                        
                        y += lineHeight;
                        data->contentHeight += lineHeight;
                        strcpy(line, token);
                    } else {
                        // Add token to current line
                        if (strlen(line) > 0) {
                            strcat(line, " ");
                            strcat(line, token);
                        } else {
                            strcpy(line, token);
                        }
                    }
                    
                    token = strtok(NULL, " \n");
                    
                    // If this was the last token, draw the final line
                    if (!token && strlen(line) > 0) {
                        if (y + lineHeight > component->bounds.y && y < component->bounds.y + component->bounds.height) {
                            DrawText(line, x, y, fontSize, data->textColor);
                        }
                        y += lineHeight;
                        data->contentHeight += lineHeight;
                    }
                }
                
                // Draw scrollbar if content exceeds bounds
                if (data->scrollable && data->contentHeight > component->bounds.height) {
                    Rectangle scrollbarBg = {
                        component->bounds.x + component->bounds.width - data->scrollbarWidth,
                        component->bounds.y,
                        data->scrollbarWidth,
                        component->bounds.height
                    };
                    DrawRectangleRec(scrollbarBg, ColorAlpha(data->scrollbarColor, 0.2f));
                    
                    // Calculate scrollbar height and position
                    float scrollRatio = component->bounds.height / data->contentHeight;
                    float scrollbarHeight = component->bounds.height * scrollRatio;
                    float scrollbarY = component->bounds.y + (data->scrollPosition / data->contentHeight) * component->bounds.height;
                    
                    // Ensure scrollbar doesn't go out of bounds
                    if (scrollbarY + scrollbarHeight > component->bounds.y + component->bounds.height) {
                        scrollbarY = component->bounds.y + component->bounds.height - scrollbarHeight;
                    }
                    
                    Rectangle scrollbar = {
                        component->bounds.x + component->bounds.width - data->scrollbarWidth,
                        scrollbarY,
                        data->scrollbarWidth,
                        scrollbarHeight
                    };
                    DrawRectangleRec(scrollbar, data->scrollbarColor);
                    
                    // Handle scrolling with mouse wheel
                    if (IsComponentHovered(component)) {
                        float wheel = GetMouseWheelMove();
                        if (wheel != 0) {
                            data->scrollPosition -= wheel * 20;
                            
                            // Clamp scrolling
                            if (data->scrollPosition < 0) {
                                data->scrollPosition = 0;
                            } else if (data->scrollPosition > data->contentHeight - component->bounds.height) {
                                data->scrollPosition = fmaxf(0, data->contentHeight - component->bounds.height);
                            }
                        }
                    }
                }
                
                free(textCopy);
            } else {
                // Draw non-wrapped text
                DrawText(data->text, component->bounds.x, component->bounds.y - data->scrollPosition, 
                        data->fontSize, data->textColor);
                
                // Calculate content height
                data->contentHeight = lineHeight;
            }
            
            EndScissorMode();
            break;
        }
        case RAYDIAL_PANEL: {
            RayDialPanelData* data = (RayDialPanelData*)component->data;
            
            // Draw panel background
            DrawRectangleRec(component->bounds, data->backgroundColor);
            
            // Draw panel border
            DrawRectangleLinesEx(component->bounds, data->borderWidth, data->borderColor);
            break;
        }
        default:
            break;
    }
    
    // Draw child components
    RayDialComponent* child = component->next;
    while (child) {
        DrawComponent(child);
        child = child->next;
    }
}

void FreeComponent(RayDialComponent* component) {
    if (!component) return;
    
    // Free child components
    RayDialComponent* child = component->next;
    while (child) {
        RayDialComponent* next = child->next;
        FreeComponent(child);
        child = next;
    }
    
    // Free component data
    free(component->data);
    free(component);
}

// Dialogue node functions
RayDialNode* CreateDialogueNode(const char* id, const char* text) {
    RayDialNode* node = (RayDialNode*)malloc(sizeof(RayDialNode));
    node->id = id;
    node->text = text;
    node->components = NULL;
    node->choices = NULL;
    node->choiceCount = 0;
    node->onEnter = NULL;
    node->onExit = NULL;
    node->userData = NULL;
    return node;
}

void AddChoice(RayDialNode* node, RayDialNode* choice) {
    if (!node || !choice) return;
    
    node->choices = (RayDialNode**)realloc(node->choices, 
        sizeof(RayDialNode*) * (node->choiceCount + 1));
    node->choices[node->choiceCount++] = choice;
}

void SetNodeCallbacks(RayDialNode* node, RayDialCallback onEnter, 
                     RayDialCallback onExit, void* userData) {
    if (!node) return;
    
    node->onEnter = onEnter;
    node->onExit = onExit;
    node->userData = userData;
}

// Dialogue manager functions
RayDialManager* CreateDialogueManager(RayDialNode* rootNode) {
    RayDialManager* manager = (RayDialManager*)malloc(sizeof(RayDialManager));
    manager->rootNode = rootNode;
    manager->currentNode = rootNode;
    manager->isActive = true;
    manager->userData = NULL;
    return manager;
}

void UpdateDialogueManager(RayDialManager* manager) {
    if (!manager || !manager->isActive || !manager->currentNode) return;
    
    // Update current node's components
    if (manager->currentNode->components) {
        UpdateComponent(manager->currentNode->components);
    }
}

void DrawDialogueManager(RayDialManager* manager) {
    if (!manager || !manager->isActive || !manager->currentNode) return;
    
    // Draw current node's components
    if (manager->currentNode->components) {
        DrawComponent(manager->currentNode->components);
    }
}

void FreeDialogueManager(RayDialManager* manager) {
    if (!manager) return;
    
    // Free all nodes (you might want to implement a more sophisticated cleanup)
    free(manager);
}

// Recursive helper function to find node by ID
static RayDialNode* FindNodeById(RayDialNode* node, const char* id) {
    if (!node || !id) return NULL;
    
    // Check if current node has the target ID
    if (strcmp(node->id, id) == 0) {
        return node;
    }
    
    // Check all child nodes
    for (int i = 0; i < node->choiceCount; i++) {
        RayDialNode* found = FindNodeById(node->choices[i], id);
        if (found) return found;
    }
    
    return NULL;
}

void TransitionToNode(RayDialManager* manager, const char* nodeId) {
    if (!manager || !nodeId) return;
    
    // Call exit callback for current node
    if (manager->currentNode && manager->currentNode->onExit) {
        manager->currentNode->onExit(manager->currentNode->userData);
    }
    
    // Find the target node
    RayDialNode* targetNode = NULL;
    if (strcmp(nodeId, "root") == 0) {
        targetNode = manager->rootNode;
    } else {
        // Start search from root node
        targetNode = FindNodeById(manager->rootNode, nodeId);
    }
    
    // Set the new current node (fallback to root if target not found)
    if (targetNode) {
        manager->currentNode = targetNode;
    } else {
        printf("Warning: Node with ID '%s' not found, returning to root.\n", nodeId);
        manager->currentNode = manager->rootNode;
    }
    
    // Call enter callback for new node
    if (manager->currentNode && manager->currentNode->onEnter) {
        manager->currentNode->onEnter(manager->currentNode->userData);
    }
}

// Utility functions
bool IsComponentClicked(RayDialComponent* component) {
    if (!component || !component->enabled) return false;
    
    // Track the last component that was clicked to prevent multiple triggers
    static RayDialComponent* lastClickedComponent = NULL;
    static int lastClickTime = -1;
    
    // Get current time to track unique clicks (in milliseconds)
    int currentTime = (int)(GetTime() * 1000.0f);
    
    Vector2 mousePos = GetMousePosition();
    bool collision = CheckCollisionPointRec(mousePos, component->bounds);
    bool clicked = collision && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    if (clicked) {
        // If this is the same component clicked in rapid succession (within 100ms), ignore it
        if (component == lastClickedComponent && (currentTime - lastClickTime < 100)) {
            return false;
        }
        
        // Update tracking
        lastClickedComponent = component;
        lastClickTime = currentTime;
        return true;
    }
    
    // If mouse button is released, reset the last clicked component
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        lastClickedComponent = NULL;
    }
    
    return false;
}

bool IsComponentHovered(RayDialComponent* component) {
    if (!component || !component->enabled) return false;
    
        Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, component->bounds);
}

void SetComponentEnabled(RayDialComponent* component, bool enabled) {
    if (component) {
        component->enabled = enabled;
    }
}

void SetComponentVisible(RayDialComponent* component, bool visible) {
    if (component) {
        component->visible = visible;
    }
}