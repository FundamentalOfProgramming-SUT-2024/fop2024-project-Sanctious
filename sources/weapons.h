#ifndef WEAPONS_H
#define WEAPONS_H

#include "item.h"

typedef enum {
    MELEEWEAPON_SWORD, // Mace included

} MeleeWeaponClass;

typedef struct{
    MeleeWeaponClass subclass;

    int damage;

} MeleeWeaponExtra;

//**********************
typedef enum {
    RANGEDWEAPON_ARROW, // Dagger included
    RANGEDWEAPON_MAGICWAND,

} RangedWeaponClass;

typedef struct{
    RangedWeaponClass subclass;

    int range;
    int reusable;
    int damage;

} RangedWeaponExtra;


Item* createMeleeWeapon(Item* baseItem, MeleeWeaponClass subclass, int damage);
Item* createRangedWeapon(Item* baseItem, RangedWeaponClass subclass, int range, int damage);

#endif
