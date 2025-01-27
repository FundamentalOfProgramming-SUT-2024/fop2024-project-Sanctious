#include "item.h"
#include "stdlib.h"
#include "foods.h"

Item* createFood(Item* baseItem, FoodClass subclass, int hunger){
    baseItem->itemclass = IC_FOOD;
    FoodExtra* extra = (FoodExtra *) malloc(1 * sizeof(FoodExtra));

    extra->subclass = subclass;
    extra->hunger = hunger;

    baseItem->ItemExtra = (void *) extra;
    return baseItem;
}

