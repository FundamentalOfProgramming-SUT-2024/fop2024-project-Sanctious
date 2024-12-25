#ifndef PLAYER_H
#define PLAYER_H

#include "item.h"

// its a bad practice but i have now other idea
#define MAX_INVENTORY_SIZE 16

typedef struct {
    int gridXPosition;
    int gridYPosition;

    int inventory_size;
    Item inventory[MAX_INVENTORY_SIZE];

    int armor;
    int gold;
    int health;


} Player;


void initializePlayer();
Player* getPlayerInstance();

#endif
