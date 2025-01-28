#include "entities.h"
#include "stdlib.h"
#include "strings.h"
#include "logger.h"
#include "renderer.h"


Entity* createEntity(char* name, gCord pos, char sprite[5], Color spriteColor){
    Entity* entity = (Entity *) malloc(1 * sizeof(Entity));

    strcpy(entity->name, name);
    strcpy(entity->sprite, sprite);
    entity->spriteColor = spriteColor;
    entity->pos = pos;

    Log("Entity created with name: \"%s\" and sprite: %c", _DEBUG_, name, sprite);
    return entity;
}

Entity* createDemon(Entity* entity){
//    baseItem->itemclass = IC_MELEEWEAPON;

    entity->entityclass = EC_DEMON;
    DemonExtra* extra = (DemonExtra *) malloc(1 * sizeof(DemonExtra));

//    extra->subclass = subclass;
//    extra->damage = damage;

    entity->EntityExtra = (void *) extra;
    return entity;
}
