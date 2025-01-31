#include "item.h"
#include "stdlib.h"
#include "potions.h"
#include "logger.h"
#include "player.h"

Item* createPotion(Item* baseItem, PotionClass subclass, int impact){
    baseItem->itemclass = IC_POTION;
    PotionExtra* extra = (PotionExtra *) malloc(1 * sizeof(PotionExtra));

    extra->subclass = subclass;
    extra->impact = impact;

    baseItem->ItemExtra = (void *) extra;

    Log("Potion generated with pos: (%d, %d) impact: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->impact);
    return baseItem;
}

int PotionOnConsume(Item* item){
    item->count--;
    PotionExtra* extra = (PotionExtra *) item->ItemExtra;




    if (item->count == 0) return 1;
    return 0;
}
