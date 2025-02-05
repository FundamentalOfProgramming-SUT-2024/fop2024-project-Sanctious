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

    switch(extra->subclass){
    case FOOD_NORMAL:
        modifyPlayerHunger(getPlayerInstance(), extra->hungerPoints);
        addEventMessage("Consumed %s %d\u0103", item->name, extra->hungerPoints);
        break;
    case FOOD_MAGICAL:
        modifyPlayerHunger(getPlayerInstance(), extra->hungerPoints);
        getPlayerInstance()->multies[1] *= 2;
        getPlayerInstance()->multiesT[1] += 10;
        addEventMessage("Consumed %s %d\u0103 2x\u010e", item->name, extra->hungerPoints);
        break;
    case FOOD_LEGENDARY:
        modifyPlayerHunger(getPlayerInstance(), extra->hungerPoints);
        getPlayerInstance()->multies[0] *= 2;
        getPlayerInstance()->multiesT[0] += 10;
        addEventMessage("Consumed %s %d\u0103 2x\u0104", item->name, extra->hungerPoints);
        break;
    case FOOD_ROTTEN:
        modifyPlayerHealth(getPlayerInstance(), -2);
        addEventMessage("Consumed %s, decreased health by 2\u0100", item->name);
        break;
    }


    if (item->count == 0) return 1;
    return 0;
}

int FoodOnPickup(Item* item){
    getPlayerInstance()->gold += item->count;

    addEventMessage("Picked up %d%s", item->count, item->sprite);
    return 1;
}
