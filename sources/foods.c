#include "item.h"
#include "stdlib.h"
#include "logger.h"
#include "foods.h"

Item* createFood(Item* baseItem, FoodClass subclass, int hunger){
    baseItem->itemclass = IC_FOOD;
    FoodExtra* extra = (FoodExtra *) malloc(1 * sizeof(FoodExtra));

    extra->subclass = subclass;
    extra->hunger = hunger;

    baseItem->ItemExtra = (void *) extra;

    Log("Food generated with pos: (%d, %d) hunger: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->hunger);
    return baseItem;
}

