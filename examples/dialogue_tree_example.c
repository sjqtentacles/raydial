#include "raylib.h"
#include "raydial.h"
#include <stdio.h>
#include <string.h>

// Game state structure
typedef struct {
    int playerHealth;
    int playerGold;
    bool hasSword;
    bool hasShield;
    bool hasPotion;
    bool hasKey;
} GameState;

// Callback for when player buys a sword
void OnBuySword(void* userData) {
    GameState* state = (GameState*)userData;
    if (state->playerGold >= 50) {
        state->playerGold -= 50;
        state->hasSword = true;
        printf("You bought a sword!\n");
    } else {
        printf("Not enough gold!\n");
    }
}

// Callback for when player buys a shield
void OnBuyShield(void* userData) {
    GameState* state = (GameState*)userData;
    if (state->playerGold >= 30) {
        state->playerGold -= 30;
        state->hasShield = true;
        printf("You bought a shield!\n");
    } else {
        printf("Not enough gold!\n");
    }
}

// Callback for when player buys a potion
void OnBuyPotion(void* userData) {
    GameState* state = (GameState*)userData;
    if (state->playerGold >= 20) {
        state->playerGold -= 20;
        state->hasPotion = true;
        printf("You bought a healing potion!\n");
    } else {
        printf("Not enough gold!\n");
    }
}

// Callback for when player finds a key
void OnFindKey(void* userData) {
    GameState* state = (GameState*)userData;
    state->hasKey = true;
    printf("You found a mysterious key!\n");
}

// Callback for when player uses a potion
void OnUsePotion(void* userData) {
    GameState* state = (GameState*)userData;
    if (state->hasPotion) {
        state->playerHealth = 100;
        state->hasPotion = false;
        printf("You used the healing potion!\n");
    } else {
        printf("You don't have a potion!\n");
    }
}

// Navigation callbacks
void OnNavigateToShop(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "shop");
}

void OnNavigateToDungeon(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "dungeon");
}

void OnNavigateToBattle(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "battle");
}

void OnNavigateToTreasure(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "treasure");
}

void OnNavigateToRoot(void* userData) {
    RayDialManager* manager = (RayDialManager*)userData;
    TransitionToNode(manager, "root");
}

int main(void) {
    // Initialize raylib window
    const int screenWidth = 1024;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "Dialogue Tree Example");
    SetTargetFPS(60);

    // Initialize game state
    GameState gameState = {
        .playerHealth = 100,
        .playerGold = 100,
        .hasSword = false,
        .hasShield = false,
        .hasPotion = false,
        .hasKey = false
    };

    // Create dialogue nodes
    RayDialNode* rootNode = CreateDialogueNode("root", "Welcome to the game!");
    RayDialNode* shopNode = CreateDialogueNode("shop", "Welcome to the shop!");
    RayDialNode* dungeonNode = CreateDialogueNode("dungeon", "You enter the dungeon...");
    RayDialNode* battleNode = CreateDialogueNode("battle", "Prepare for battle!");
    RayDialNode* treasureNode = CreateDialogueNode("treasure", "You found a treasure room!");
    
    // Create dialogue manager
    RayDialManager* manager = CreateDialogueManager(rootNode);
    
    // Connect nodes in the dialogue tree
    AddChoice(rootNode, shopNode);
    AddChoice(rootNode, dungeonNode);
    AddChoice(dungeonNode, battleNode);
    AddChoice(dungeonNode, treasureNode);
    
    // Set up root node
    RayDialComponent* rootPanel = CreatePanel(
        (Rectangle){ 100, 100, 824, 568 },
        LIGHTGRAY
    );
    
    RayDialComponent* rootTitle = CreateLabel(
        (Rectangle){ 120, 120, 784, 40 },
        "Welcome to the Adventure!",
        true  // Enable text wrapping
    );
    
    RayDialComponent* rootMessage = CreateLabel(
        (Rectangle){ 120, 180, 784, 40 },
        "Choose your path:",
        true  // Enable text wrapping
    );
    
    RayDialComponent* shopButton = CreateButton(
        (Rectangle){ 120, 240, 300, 50 },
        "Visit the Shop",
        OnNavigateToShop,
        manager
    );
    
    RayDialComponent* dungeonButton = CreateButton(
        (Rectangle){ 120, 310, 300, 50 },
        "Enter the Dungeon",
        OnNavigateToDungeon,
        manager
    );
    
    AddComponent(rootPanel, rootTitle);
    AddComponent(rootPanel, rootMessage);
    AddComponent(rootPanel, shopButton);
    AddComponent(rootPanel, dungeonButton);
    rootNode->components = rootPanel;
    
    // Set up shop node
    RayDialComponent* shopPanel = CreatePanel(
        (Rectangle){ 100, 100, 824, 568 },
        LIGHTGRAY
    );
    
    RayDialComponent* shopTitle = CreateLabel(
        (Rectangle){ 120, 120, 784, 40 },
        "Welcome to the Shop!",
        false
    );
    
    RayDialComponent* goldLabel = CreateLabel(
        (Rectangle){ 120, 180, 784, 40 },
        "Your gold: 100",
        true  // Enable text wrapping
    );
    
    RayDialComponent* swordButton = CreateButton(
        (Rectangle){ 120, 240, 300, 50 },
        "Buy Sword (50g)",
        OnBuySword,
        &gameState
    );
    
    RayDialComponent* shieldButton = CreateButton(
        (Rectangle){ 120, 310, 300, 50 },
        "Buy Shield (30g)",
        OnBuyShield,
        &gameState
    );
    
    RayDialComponent* potionButton = CreateButton(
        (Rectangle){ 120, 380, 300, 50 },
        "Buy Potion (20g)",
        OnBuyPotion,
        &gameState
    );
    
    RayDialComponent* backButton = CreateButton(
        (Rectangle){ 120, 450, 300, 50 },
        "Back to Main Menu",
        OnNavigateToRoot,
        manager
    );
    
    AddComponent(shopPanel, shopTitle);
    AddComponent(shopPanel, goldLabel);
    AddComponent(shopPanel, swordButton);
    AddComponent(shopPanel, shieldButton);
    AddComponent(shopPanel, potionButton);
    AddComponent(shopPanel, backButton);
    shopNode->components = shopPanel;
    
    // Set up dungeon node
    RayDialComponent* dungeonPanel = CreatePanel(
        (Rectangle){ 100, 100, 824, 568 },
        DARKGRAY
    );
    
    RayDialComponent* dungeonTitle = CreateLabel(
        (Rectangle){ 120, 120, 784, 40 },
        "You enter the dungeon...",
        true  // Enable text wrapping
    );
    
    RayDialComponent* dungeonMessage = CreateLabel(
        (Rectangle){ 120, 180, 784, 80 },
        "The dungeon is dark and damp. You hear strange noises echoing through the halls. What will you do?",
        true  // Enable text wrapping
    );
    
    RayDialComponent* battleButton = CreateButton(
        (Rectangle){ 120, 240, 300, 50 },
        "Fight the Monster",
        OnNavigateToBattle,
        manager
    );
    
    RayDialComponent* treasureButton = CreateButton(
        (Rectangle){ 120, 310, 300, 50 },
        "Search for Treasure",
        OnNavigateToTreasure,
        manager
    );
    
    RayDialComponent* dungeonBackButton = CreateButton(
        (Rectangle){ 120, 380, 300, 50 },
        "Return to Surface",
        OnNavigateToRoot,
        manager
    );
    
    AddComponent(dungeonPanel, dungeonTitle);
    AddComponent(dungeonPanel, dungeonMessage);
    AddComponent(dungeonPanel, battleButton);
    AddComponent(dungeonPanel, treasureButton);
    AddComponent(dungeonPanel, dungeonBackButton);
    dungeonNode->components = dungeonPanel;
    
    // Set up battle node
    RayDialComponent* battlePanel = CreatePanel(
        (Rectangle){ 100, 100, 824, 568 },
        DARKGRAY
    );
    
    RayDialComponent* battleTitle = CreateLabel(
        (Rectangle){ 120, 120, 784, 40 },
        "Prepare for battle!",
        true  // Enable text wrapping
    );
    
    RayDialComponent* battleMessage = CreateLabel(
        (Rectangle){ 120, 180, 784, 80 },
        "A fierce monster appears before you! You must fight to survive!",
        true  // Enable text wrapping
    );
    
    RayDialComponent* healthLabel = CreateLabel(
        (Rectangle){ 120, 240, 784, 40 },
        "Health: 100",
        false
    );
    
    RayDialComponent* equipmentLabel = CreateLabel(
        (Rectangle){ 120, 240, 784, 40 },
        "Equipment: None",
        false
    );
    
    RayDialComponent* usePotionButton = CreateButton(
        (Rectangle){ 120, 310, 300, 50 },
        "Use Healing Potion",
        OnUsePotion,
        &gameState
    );
    
    RayDialComponent* battleBackButton = CreateButton(
        (Rectangle){ 120, 380, 300, 50 },
        "Retreat to Dungeon",
        OnNavigateToDungeon,
        manager
    );
    
    AddComponent(battlePanel, battleTitle);
    AddComponent(battlePanel, healthLabel);
    AddComponent(battlePanel, equipmentLabel);
    AddComponent(battlePanel, usePotionButton);
    AddComponent(battlePanel, battleBackButton);
    battleNode->components = battlePanel;
    
    // Set up treasure node
    RayDialComponent* treasurePanel = CreatePanel(
        (Rectangle){ 100, 100, 824, 568 },
        GOLD
    );
    
    RayDialComponent* treasureTitle = CreateLabel(
        (Rectangle){ 120, 120, 784, 40 },
        "You found a treasure room!",
        false
    );
    
    RayDialComponent* treasureMessage = CreateLabel(
        (Rectangle){ 120, 180, 784, 40 },
        "There's a locked chest and a mysterious key on the ground.",
        false
    );
    
    RayDialComponent* findKeyButton = CreateButton(
        (Rectangle){ 120, 240, 300, 50 },
        "Pick up the Key",
        OnFindKey,
        &gameState
    );
    
    RayDialComponent* treasureBackButton = CreateButton(
        (Rectangle){ 120, 310, 300, 50 },
        "Return to Dungeon",
        OnNavigateToDungeon,
        manager
    );
    
    AddComponent(treasurePanel, treasureTitle);
    AddComponent(treasurePanel, treasureMessage);
    AddComponent(treasurePanel, findKeyButton);
    AddComponent(treasurePanel, treasureBackButton);
    treasureNode->components = treasurePanel;
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update
        UpdateDialogueManager(manager);
        
        // Update UI text
        char goldText[32];
        snprintf(goldText, sizeof(goldText), "Your gold: %d", gameState.playerGold);
        ((RayDialLabelData*)goldLabel->data)->text = goldText;
        
        char healthText[32];
        snprintf(healthText, sizeof(healthText), "Health: %d", gameState.playerHealth);
        ((RayDialLabelData*)healthLabel->data)->text = healthText;
        
        char equipmentText[128];
        snprintf(equipmentText, sizeof(equipmentText), "Equipment: %s%s%s%s%s%s%s",
            gameState.hasSword ? "Sword" : "",
            (gameState.hasSword && (gameState.hasShield || gameState.hasPotion || gameState.hasKey)) ? ", " : "",
            gameState.hasShield ? "Shield" : "",
            ((gameState.hasSword || gameState.hasShield) && (gameState.hasPotion || gameState.hasKey)) ? ", " : "",
            gameState.hasPotion ? "Potion" : "",
            ((gameState.hasSword || gameState.hasShield || gameState.hasPotion) && gameState.hasKey) ? ", " : "",
            gameState.hasKey ? "Key" : "None");
        ((RayDialLabelData*)equipmentLabel->data)->text = equipmentText;
        
        // Draw
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawDialogueManager(manager);
        EndDrawing();
    }
    
    // Cleanup
    FreeDialogueManager(manager);
    CloseWindow();
    return 0;
} 