#ifndef WEAPONS_H
#define WEAPONS_H

typedef struct Item Item;

typedef enum {
    MELEEWEAPON_SWORD,
    MELEEWEAPON_MACE,

} MeleeWeaponClass;

typedef struct{
    MeleeWeaponClass subclass;

    int damage;

} MeleeWeaponExtra;

//**********************
typedef enum {
    RANGEDWEAPON_ARROW,
    RANGEDWEAPON_DAGGER,
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
int MWeaponOnAttack(Item* item);
int RWeaponOnAttack(Item* item);
int MWeaponOnPickup(Item* item);
int RWeaponOnPickup(Item* item);

#endif
