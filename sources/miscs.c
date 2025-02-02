#include "item.h"
#include "stdlib.h"
#include "logger.h"
#include "miscs.h"
#include "player.h"

Item* createGold(Item* baseItem, GoldClass subclass){
    baseItem->itemclass = IC_GOLD;

    GoldExtra* extra = (GoldExtra *) malloc(1 * sizeof(GoldExtra));

    extra->subclass = subclass;

    baseItem->ItemExtra = (void *) extra;

    Log("Gold generated with pos: (%d, %d) count: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, baseItem->count);
    return baseItem;
}

int GoldOnPickup(Item* item){
    getPlayerInstance()->gold += item->count;

    return 1;
}
