#ifndef PLAYER_H
#define PLAYER_H

#include "item.h"
#include "map.h"

// its a bad practice but i have now other idea
#define MAX_INVENTORY_SIZE 16

typedef struct {
    gCord pos;

    int inventory_size;
    Item inventory[MAX_INVENTORY_SIZE];

    int armor;
    int gold;
    int health;


} Player;


void initializePlayer(Player*);
Player* getPlayerInstance();
Room* findPlayerRoom();
char isValidPos(int , int);

#endif
