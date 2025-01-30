#ifndef ITEM_H
#define ITEM_H

#include "config.h"
#include "renderer.h"
#include "gc.h"

typedef enum {
    // Item Type consumables
    IC_FOOD         = 2, // Used in displaying inventory
    IC_POTION       = 3,
    IC_MELEEWEAPON  = 0,
    IC_RANGEDWEAPON = 1,
} ItemClass;

typedef struct {
    char name[MAX_STR_SIZE];
    char sprite[5]; // Custom sprite support
    Color spriteColor;
    gCord pos;
    int count;


    ItemClass itemclass;
    void* ItemExtra;
} Item;

Item* createBaseItem(char* name, gCord pos, char sprite[5], Color spriteColor, int count);

#endif
