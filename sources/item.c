#include "item.h"
#include "stdlib.h"
#include "strings.h"
#include "logger.h"
#include "renderer.h"
#include "map.h"

Item* createBaseItem(char* name, gCord pos, char sprite[5], Color spriteColor, int count){
    Item* baseItem = (Item *) malloc(1 * sizeof(Item));

    strcpy(baseItem->name, name);
    strcpy(baseItem->sprite, sprite);
    baseItem->spriteColor = spriteColor;
    baseItem->count = count;
    baseItem->pos = pos;

    if (baseItem->count == -1){


    }

    return baseItem;
}
