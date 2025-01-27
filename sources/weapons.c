#include "item.h"
#include "stdlib.h"
#include "weapons.h"

Item* createMeleeWeapon(Item* baseItem, MeleeWeaponClass subclass, int damage){
    baseItem->itemclass = IC_MELEEWEAPON;
    MeleeWeaponExtra* extra = (MeleeWeaponExtra *) malloc(1 * sizeof(MeleeWeaponExtra));

    extra->subclass = subclass;
    extra->damage = damage;

    baseItem->ItemExtra = (void *) extra;    return baseItem;
}

Item* createRangedWeapon(Item* baseItem, RangedWeaponClass subclass, int range, int damage){
    baseItem->itemclass = IC_RANGEDWEAPON;
    RangedWeaponExtra* extra = (RangedWeaponExtra*) malloc(1 * sizeof(RangedWeaponExtra));

    extra->subclass = subclass;
    extra->range = range;
    extra->reusable = 1;
    extra->damage = damage;

    baseItem->ItemExtra = (void *) extra;
    return baseItem;
}
