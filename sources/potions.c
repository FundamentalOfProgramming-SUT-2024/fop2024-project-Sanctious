#include "item.h"
#include "stdlib.h"
#include "potions.h"
#include "logger.h"

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

