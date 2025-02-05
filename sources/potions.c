#include "item.h"
#include "stdlib.h"
#include "potions.h"
#include "logger.h"
#include "player.h"
#include "scenes/s_game.h"

Item* createPotion(Item* baseItem, PotionClass subclass, int impact, int duration){
    baseItem->itemclass = IC_POTION;
    PotionExtra* extra = (PotionExtra *) malloc(1 * sizeof(PotionExtra));

    extra->subclass = subclass;
    extra->impact = impact;
    extra->duration = duration;

    baseItem->ItemExtra = (void *) extra;

    Log("Potion generated with pos: (%d, %d) impact: %d.", _DEBUG_,
             baseItem->pos.gridX, baseItem->pos.gridY, extra->impact);
    return baseItem;
}

int PotionOnConsume(Item* item){
    item->count--;
    PotionExtra* extra = (PotionExtra *) item->ItemExtra;
    Player* player = getPlayerInstance();

    switch(extra->subclass){
    case POTION_STRENGTH:
        addEventMessage("Consumed Strength potion%s x%d for %ds", item->sprite, extra->impact, extra->duration);
        player->multies[0] *= extra->impact;
        player->multiesT[0] += extra->duration;
        break;

    case POTION_HEAL:
        addEventMessage("Consumed Heal potion%s x%d for %ds", item->sprite, extra->impact, extra->duration);
        player->multies[2] *= extra->impact;
        player->multiesT[2] += extra->duration;
        break;

    case POTION_SPEED:
        addEventMessage("Consumed Speed potion%s x%d for %ds", item->sprite, extra->impact, extra->duration);
        player->multies[1] *= extra->impact;
        player->multiesT[1] += extra->duration;
        break;
    }


    if (item->count == 0) return 1;
    return 0;
}

int PotionOnPickup(Item* item){
    addEventMessage("Picked up %d%s", item->count, item->sprite);
    return 1;
}
