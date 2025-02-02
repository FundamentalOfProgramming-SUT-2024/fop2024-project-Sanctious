#include "item.h"
#include "stdlib.h"
#include "logger.h"
#include "foods.h"
#include "player.h"
#include "scenes/s_game.h"

Item* createFood(Item* baseItem, FoodClass subclass, int hungerPoints){
    baseItem->itemclass = IC_FOOD;
    FoodExtra* extra = (FoodExtra *) malloc(1 * sizeof(FoodExtra));

    extra->subclass = subclass;
    extra->hungerPoints = hungerPoints;

    baseItem->ItemExtra = (void *) extra;

    Log("Food generated with pos: (%d, %d) hungerPoints: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->hungerPoints);
    return baseItem;
}

int FoodOnConsume(Item* item){
    item->count--;
    FoodExtra* extra = (FoodExtra *) item->ItemExtra;

    modifyPlayerHunger(getPlayerInstance(), extra->hungerPoints);


    addEventMessage("Consumed Food %d\u0103", extra->hungerPoints);
    if (item->count == 0) return 1;
    return 0;
}

int FoodOnPickup(Item* item){
    getPlayerInstance()->gold += item->count;

    addEventMessage("Picked up %d%s", item->count, item->sprite);
    return 1;
}
