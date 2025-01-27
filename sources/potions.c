#include "item.h"
#include "stdlib.h"
#include "potions.h"

Item* createPotion(Item* baseItem, PotionClass subclass, int impact){
    baseItem->itemclass = IC_POTION;
    PotionExtra* extra = (PotionExtra *) malloc(1 * sizeof(PotionExtra));

    extra->subclass = subclass;
    extra->impact = impact;

    baseItem->ItemExtra = (void *) extra;
    return baseItem;
}

