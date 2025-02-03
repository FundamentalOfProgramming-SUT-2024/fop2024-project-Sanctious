#include "item.h"
#include "stdlib.h"
#include "string.h"
#include "logger.h"
#include "renderer.h"
#include "map.h"
#include "allitems.h"
#include "scenes/s_game.h"

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
        addEventMessage("Picked up %d%s", item->count, item->sprite);
        return GoldOnPickup(item);
    }
    default:{
        addEventMessage("Picked up %s %d%s", item->name, item->count, item->sprite);
        break;
    }
//    }
//    case IC_FOOD:{
//        return FoodOnPickup(item);
//    }
//    case IC_POTION:{
//        return PotionOnPickup(item);
//    }
//    case IC_MELEEWEAPON:{
//        return MWeaponOnPickup(item);
//    }
//    case IC_RANGEDWEAPON:{
//        return RWeaponOnPickup(item);
    }

    return 0;
}

int ItemOnAttack(Item* item){
    if (item == NULL) return 0;

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
