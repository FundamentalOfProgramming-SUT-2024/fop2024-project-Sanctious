#include "item.h"
#include "stdlib.h"
#include "weapons.h"
#include "logger.h"
#include "map.h"
#include "player.h"
#include "scenes/s_game.h"

Item* createMeleeWeapon(Item* baseItem, MeleeWeaponClass subclass, int damage){
    baseItem->itemclass = IC_MELEEWEAPON;
    MeleeWeaponExtra* extra = (MeleeWeaponExtra *) malloc(1 * sizeof(MeleeWeaponExtra));

    extra->subclass = subclass;
    extra->damage = damage;

    baseItem->ItemExtra = (void *) extra;

    Log("MWeapon generated with pos: (%d, %d) damage: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->damage);
    return baseItem;
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
    MeleeWeaponExtra* extra = (MeleeWeaponExtra *) player->equippedItem->ItemExtra;

    for (int i = 0; i < map->num_entities; ){
        Entity* entity = map->entities[i];
        if (abs(player->pos.gridX-entity->pos.gridX) <= 1
            && abs(player->pos.gridY-entity->pos.gridY) <= 1){
            addEventMessage("You dealt %d%s to %s%s", extra->damage * player->multies[0], item->sprite, entity->name, entity->sprite);
            entity->health -= extra->damage * player->multies[0];
            if (entity->health <= 0){
                removeEntityFromMap(map, i);
                addEventMessage("You killed %s%s", entity->name, entity->sprite);
                free(entity);
            }
            else{
                i++;
            }
        }
        else{
            i++;
        }
    }
    return 0;
}

int RWeaponOnAttack(Item* item){
    Player* player = getPlayerInstance();
    Map* map = getFloor(getCurFloor());
    RangedWeaponExtra* extra = (RangedWeaponExtra *) item->ItemExtra;


//    Item* _item = createBaseItem(_item->name, , _item->sprite, _item->spriteColor, 1);
//    room->items[room->num_items++] = createRangedWeapon(_item, extra->subclass, extra->range, extra->damage);
    return 0;
}

int MWeaponOnPickup(Item* item){
    addEventMessage("Picked up %d%s", item->count, item->sprite);
    return 1;
}

int RWeaponOnPickup(Item* item){
    addEventMessage("Picked up %d%s", item->count, item->sprite);
    return 1;
}


