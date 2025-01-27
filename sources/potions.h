#ifndef POTIONS_H
#define POTIONS_H

#include "item.h"

typedef enum {
    POTION_HEAL,
    POTION_SPEED,  // Speed amplifier
    POTION_DAMAGE, // Damage amplifier

} PotionClass;


typedef struct{
    PotionClass subclass;

    int impact;

} PotionExtra;

Item* createPotion(Item* baseItem, PotionClass subclass, int impact);

#endif
