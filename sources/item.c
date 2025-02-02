#include "item.h"
#include "stdlib.h"
#include "strings.h"
#include "logger.h"
#include "renderer.h"
#include "map.h"
#include "allitems.h"

Item* createBaseItem(char* name, gCord pos, char sprite[5], Color spriteColor, int count){
    Item* baseItem = (Item *) malloc(1 * sizeof(Item));

    strcpy(baseItem->name, name);
    strcpy(baseItem->sprite, sprite);
    baseItem->spriteColor = spriteColor;
    baseItem->count = count;
    baseItem->pos = pos;

    if (baseItem->count == -1){


    }

    return baseItem;
}

int ItemOnConsume(Item* item){
    switch(item->itemclass){
    case IC_FOOD:{
        return FoodOnConsume(item);
    }
    case IC_POTION:{
        return PotionOnConsume(item);
    }
    }

    return 0;
}

int ItemOnPickup(Item* item){
    switch(item->itemclass){
    case IC_GOLD:{
        return GoldOnPickup(item);
    }
    }

    return 0;
}

int ItemOnAttack(Item* item){
    switch(item->itemclass){
    case IC_MELEEWEAPON:{
        return MWeaponOnAttack(item);
    }
    case IC_RANGEDWEAPON:{
        return RWeaponOnAttack(item);
    }
    }

    return 0;
}
