#ifndef ITEM_H
#define ITEM_H

#include "config.h"
#include "gc.h"


//#include "map.h"

/*

    Damage multiplier ( get damage multiplier )

*/
typedef enum {
    // Item Type consumables
    IC_FOOD,
    IC_POTION,
    IC_MELEEWEAPON,
    IC_RANGEDWEAPON,
} ItemClass;

typedef struct {
    char name[MAX_STR_SIZE];
    char sprite;
    gCord pos;
    int count;


    ItemClass itemclass;
    void* ItemExtra;
} Item;

Item* createBaseItem(char* name, gCord pos, char sprite, int count);

#endif
