#include "item.h"
#include "stdlib.h"
#include "strings.h"
#include "logger.h"
#include "map.h"

Item* createBaseItem(char* name, gCord pos, char sprite, int count){
    Item* baseItem = (Item *) malloc(1 * sizeof(Item));

    strcpy(baseItem->name, name);
    baseItem->sprite = sprite;
    baseItem->count = count;
    baseItem->pos = pos;

    if (baseItem->count == -1){


    }

    Log("Item created with name: \"%s\" and sprite: %c", _DEBUG_, name, sprite);
    return baseItem;
}
