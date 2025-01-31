#include "item.h"
#include "stdlib.h"
#include "weapons.h"
#include "logger.h"
#include "map.h"
#include "player.h"

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

int MWeaponOnAttack(Item* item){
    Player* player = getPlayerInstance();
    Map* map = getFloor(getCurFloor());

    for (int i = 0; i < map->num_entities; i++){
        Entity* entity = map->entities[i];
        if (abs(player->pos.gridX-entity->pos.gridX) <= 1
            && abs(player->pos.gridY-entity->pos.gridY) <= 1){
            entity->health -= ((MeleeWeaponExtra *) player->equippedItem->ItemExtra)->damage;
            if (entity->health <= 0){
                removeEntityFromMap(map, i);
                free(entity);
                break;
            }
        }
    }
    return 0;
}

int RWeaponOnAttack(Item* item){
    Player* player = getPlayerInstance();
    Map* map = getFloor(getCurFloor());

    return 0;
}
