#ifndef PLAYER_H
#define PLAYER_H

#include "item.h"
#include "map.h"
#include "config.h"

typedef struct {
    gCord pos;

    Item* equippedItem;

    int inventory_size;
    Item* inventory[MAX_INVENTORY_SIZE];

    int armor;
    int gold;
    int health;
    int hunger;

} Player;


void initializePlayer(Player*);
Player* getPlayerInstance();
void setPlayerInstance(Player* player);
Room* findPlayerRoom();
char isValidPos(int , int);
void removeItemFromPlayer(Player* player, int itemIndex);
void addItemToPlayer(Player* player, Item* item);

#endif
