#include "item.h"
#include "stdlib.h"
#include "weapons.h"
#include "logger.h"
#include "map.h"
#include "player.h"
#include "utils.h"
#include "scenes/s_game.h"

extern int shooting;
extern Direction shootDir;

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
        if (distancePosX(player->pos, entity->pos) <= 1 &&
            distancePosY(player->pos, entity->pos) <= 1){
            entity->health -= extra->damage * player->multies[0];
            if (entity->health <= 0){
                removeEntityFromMap(map, i);
                addEventMessage("You killed %s%s", entity->name, entity->sprite);
                free(entity);
            }
            else{
                i++;
                addEventMessage("You dealt %d%s to %s%s, has %d\u0100 left", extra->damage * player->multies[0], item->sprite, entity->name, entity->sprite, entity->health);
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

    if (shootDir != NONE){
        gCord startPos = player->pos;
        Entity* entity = NULL;
        gCord endPos;

        switch(shootDir){
        case DOWN: {
            int i = 0;
            for (; i < extra->range; i++){
                startPos.gridY++;
                if (findEntityByPosition(startPos) != NULL){
                    entity = findEntityByPosition(startPos);
                    break;
                }
                else if (!isInsideRoom(startPos)){
                    startPos.gridY--;
                    endPos = startPos;
                    break;

                }
            }
            if (i == extra->range){
                endPos = startPos;
            }
            break;
        }
        case UP:{
            int i = 0;
            for (; i < extra->range; i++){
                startPos.gridY--;
                if (findEntityByPosition(startPos) != NULL){
                    entity = findEntityByPosition(startPos);
                    break;
                }
                else if (!isInsideRoom(startPos)){
                    startPos.gridY++;
                    endPos = startPos;
                    break;

                }
            }
            if (i == extra->range){
                endPos = startPos;
            }
            break;

        }
        case LEFT:{
            int i = 0;
            for (; i < extra->range; i++){
                startPos.gridX--;
                if (findEntityByPosition(startPos) != NULL){
                    entity = findEntityByPosition(startPos);
                    break;
                }
                else if (!isInsideRoom(startPos)){
                    startPos.gridX++;
                    endPos = startPos;
                    break;

                }
            }
            if (i == extra->range){
                endPos = startPos;
            }
            break;

        }
        case RIGHT:{
            int i = 0;
            for (; i < extra->range; i++){
                startPos.gridX++;
                if (findEntityByPosition(startPos) != NULL){
                    entity = findEntityByPosition(startPos);
                    break;
                }
                else if (!isInsideRoom(startPos)){
                    startPos.gridX--;
                    endPos = startPos;
                    break;

                }
            }
            if (i == extra->range){
                endPos = startPos;
            }
            break;

        }

        }

        if (entity != NULL){
            player->equippedItem->count--;
            if (player->equippedItem->count <= 0){
                removeItemFromPlayer(player, findItemIndex(player, item));
                player->equippedItem = NULL;
                addEventMessage("You used up all of your %s%s", item->name, item->sprite);
            }
            if (extra->subclass == RANGEDWEAPON_MAGICWAND){
                entity->frozen = 1;
                addEventMessage("You froze %s%s in place", entity->name, entity->sprite);
            }

            entity->health -= extra->damage * player->multies[0];
            if (entity->health <= 0){
                removeEntityFromMap(map, findEntityIndex(map, entity));
                addEventMessage("You killed %s%s", entity->name, entity->sprite);
                free(entity);
            }
            else{
                addEventMessage("You dealt %d%s to %s%s, has %d\u0100 left", extra->damage * player->multies[0], item->sprite, entity->name, entity->sprite, entity->health);
            }
        }
        else{
            player->equippedItem->count--;
            if (player->equippedItem->count <= 0){
                removeItemFromPlayer(player, findItemIndex(player, item));
                player->equippedItem = NULL;
                addEventMessage("You used up all of your %s%s", item->name, item->sprite);
            }
            if (extra->subclass == RANGEDWEAPON_DAGGER){
                Item* temp = createBaseItem(item->name, endPos, item->sprite, item->spriteColor, 1);
                addItemToRoom(findPlayerRoom(), createRangedWeapon(temp, extra->subclass, extra->range, extra->damage));
                addEventMessage("You hit a wall!");
            }
        }

    }


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


