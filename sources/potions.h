#ifndef POTIONS_H
#define POTIONS_H

typedef struct Item Item;

typedef enum {
    POTION_HEAL,
    POTION_SPEED,  // Speed amplifier
    POTION_DAMAGE, // Damage amplifier
} PotionClass;


typedef struct{
    PotionClass subclass;

    int impact;
    int duration;

} PotionExtra;

Item* createPotion(Item* baseItem, PotionClass subclass, int impact, int duration);
int PotionOnConsume(Item* item);

#endif
