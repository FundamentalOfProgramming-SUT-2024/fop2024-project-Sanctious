#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"
#include "config.h"

typedef struct Item Item;

typedef struct {
    gCord pos;
    char sprite[5]; // Custom sprite support
    Color spriteColor;

    Item* equippedItem;

    int inventory_size;
    Item* inventory[MAX_INVENTORY_SIZE];

    int combatTagTime;
    int armor;
    int maxArmor;
    int health;
    int maxHealth;
    int hunger;
    int maxHunger;
    int gold;
    int score;

    // Damage, Speed, Regen
    int multies[3];
    int multiesT[3];
    int def_multies[3];

} Player;


void initializePlayer(Player*);
Player* getPlayerInstance();
void setPlayerInstance(Player* player);
Room* findPlayerRoom();
Corridor* findPlayerCorridor();
char isValidPos(int , int);
void removeItemFromPlayer(Player* player, int itemIndex);
void addItemToPlayer(Player* player, Item* item);

void modifyPlayerHealth(Player* player, int offset);
void modifyPlayerArmor(Player* player, int offset);
void modifyPlayerHunger(Player* player, int offset);
#endif
