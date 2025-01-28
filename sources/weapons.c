#include "item.h"
#include "stdlib.h"
#include "weapons.h"
#include "logger.h"

Item* createMeleeWeapon(Item* baseItem, MeleeWeaponClass subclass, int damage){
    baseItem->itemclass = IC_MELEEWEAPON;
    MeleeWeaponExtra* extra = (MeleeWeaponExtra *) malloc(1 * sizeof(MeleeWeaponExtra));

    extra->subclass = subclass;
    extra->damage = damage;

    baseItem->ItemExtra = (void *) extra;

    Log("MWeapon generated with pos: (%d, %d) damage: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->damage);    return baseItem;
}

Item* createRangedWeapon(Item* baseItem, RangedWeaponClass subclass, int range, int damage){
    baseItem->itemclass = IC_RANGEDWEAPON;
    RangedWeaponExtra* extra = (RangedWeaponExtra*) malloc(1 * sizeof(RangedWeaponExtra));

    extra->subclass = subclass;
    extra->range = range;
    extra->reusable = 1;
    extra->damage = damage;

    baseItem->ItemExtra = (void *) extra;

    Log("RWeapon generated with pos: (%d, %d) damage: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->damage);
    return baseItem;
}
