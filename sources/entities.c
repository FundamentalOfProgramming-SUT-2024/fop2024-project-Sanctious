#include "entities.h"
#include "stdlib.h"
#include "string.h"
#include "logger.h"
#include "renderer.h"
#include "player.h"
#include "utils.h"
#include "scenes/s_game.h"
#include "map.h"

extern const int odirs[8][2];

Room* findEntityRoom(Entity* entity){
    Map* map = getFloor(getCurFloor());

    for (int i = 0; i < map->num_rooms; i++){
        if (map->rooms[i]->pos.gridX <= entity->pos.gridX &&
            entity->pos.gridX < map->rooms[i]->scale.gridW+map->rooms[i]->pos.gridX &&
            map->rooms[i]->pos.gridY <= entity->pos.gridY &&
            entity->pos.gridY < map->rooms[i]->scale.gridH+map->rooms[i]->pos.gridY)

            return map->rooms[i];
    }
    return NULL;
}

Entity* createEntity(char* name, int maxHealth, gCord pos, char sprite[5], Color spriteColor){
    Entity* entity = (Entity *) malloc(1 * sizeof(Entity));

    strcpy(entity->name, name);
    strcpy(entity->sprite, sprite);
    entity->spriteColor = spriteColor;
    entity->pos = pos;

    entity->goldDrop = 0;
    entity->armor = 5;
    entity->maxArmor = 5;
    entity->maxHealth = maxHealth;
    entity->health = maxHealth;

    return entity;
}

Entity* createDemon(Entity* entity, int damage, int maxfp){
    entity->entityclass = EC_DEMON;
    DemonExtra* extra = (DemonExtra *) malloc(1 * sizeof(DemonExtra));

//    extra->subclass = subclass;
    extra->followPlayer = maxfp;
    extra->maxfp = maxfp;
    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Demon created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createSnake(Entity* entity, int damage){
    entity->entityclass = EC_SNAKE;
    SnakeExtra* extra = (SnakeExtra *) malloc(1 * sizeof(SnakeExtra));

//    extra->subclass = subclass;
    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Snake created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createGiant(Entity* entity, int damage, int maxfp){
    entity->entityclass = EC_GIANT;
    GiantExtra* extra = (GiantExtra *) malloc(1 * sizeof(GiantExtra));

//    extra->subclass = subclass;
    extra->followPlayer = maxfp;
    extra->maxfp = maxfp;
    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Giant created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createDragon(Entity* entity, int damage, int maxfp){
    entity->entityclass = EC_DRAGON;
    DragonExtra* extra = (DragonExtra *) malloc(1 * sizeof(DragonExtra));

//    extra->subclass = subclass;
    extra->followPlayer = maxfp;
    extra->maxfp = maxfp;
    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Dragon created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createUndead(Entity* entity, int damage, int maxfp){
    entity->entityclass = EC_UNDEAD;
    UndeadExtra* extra = (UndeadExtra *) malloc(1 * sizeof(UndeadExtra));

//    extra->subclass = subclass;
    extra->followPlayer = maxfp;
    extra->maxfp = maxfp;
    extra->damage = damage;
    extra->triggered = 0;

    entity->EntityExtra = (void *) extra;

    Log("Undead created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

void DemonOnAction(Entity* entity){
    DemonExtra* extra = (DemonExtra *) entity->EntityExtra;
    Player* player = getPlayerInstance();

    if (findPlayerRoom() == (Room *) findEntityRoom(entity)){
        if (extra->followPlayer <= 0){
            return;
        }
        else{
            extra->followPlayer--;
        }
        int xuvector = 1;
        if (player->pos.gridX-entity->pos.gridX < 0) xuvector = -1;
        else if (player->pos.gridX-entity->pos.gridX == 0) xuvector = 0;
        int yuvector = 1;
        if (player->pos.gridY-entity->pos.gridY < 0) yuvector = -1;
        else if (player->pos.gridY-entity->pos.gridY == 0) yuvector = 0;

        if (distancePosX(player->pos, entity->pos) <= 1 &&
            distancePosY(player->pos, entity->pos) <= 1){
            xuvector = 0;
            yuvector = 0;
            modifyPlayerHealth(player, -extra->damage);
            addEventMessage("Entity %s%s damaged you by %d\u0100", entity->name, entity->sprite, extra->damage);
        }

        entity->pos.gridX += xuvector;
        entity->pos.gridY += yuvector;

        if (extra->followPlayer == 0){
            addEventMessage("%s%s turns away, no longer interested in you", entity->name, entity->sprite);
        }
    }
    else{
        extra->followPlayer = extra->maxfp;
    }
}

void DragonOnAction(Entity* entity){
    DragonExtra* extra = (DragonExtra *) entity->EntityExtra;
    Player* player = getPlayerInstance();

    if (findPlayerRoom() == (Room *) findEntityRoom(entity)){
        if (extra->followPlayer <= 0){
            return;
        }
        else{
            extra->followPlayer--;
        }
        int xuvector = 1;
        if (player->pos.gridX-entity->pos.gridX < 0) xuvector = -1;
        else if (player->pos.gridX-entity->pos.gridX == 0) xuvector = 0;
        int yuvector = 1;
        if (player->pos.gridY-entity->pos.gridY < 0) yuvector = -1;
        else if (player->pos.gridY-entity->pos.gridY == 0) yuvector = 0;

        if (distancePosX(player->pos, entity->pos) <= 1 &&
            distancePosY(player->pos, entity->pos) <= 1){
            xuvector = 0;
            yuvector = 0;
            modifyPlayerHealth(player, -extra->damage);
            addEventMessage("Entity %s%s damaged you by %d\u0100", entity->name, entity->sprite, extra->damage);
        }

        entity->pos.gridX += xuvector;
        entity->pos.gridY += yuvector;

        if (extra->followPlayer == 0){
            addEventMessage("%s%s turns away, no longer interested in you", entity->name, entity->sprite);
        }
    }
    else{
        extra->followPlayer = extra->maxfp;
    }
}

void GiantOnAction(Entity* entity){
    GiantExtra* extra = (GiantExtra *) entity->EntityExtra;
    Player* player = getPlayerInstance();

    if (findPlayerRoom() == (Room *) findEntityRoom(entity)){
        if (extra->followPlayer <= 0){
            return;
        }
        else{
            extra->followPlayer--;
        }
        int xuvector = 1;
        if (player->pos.gridX-entity->pos.gridX < 0) xuvector = -1;
        else if (player->pos.gridX-entity->pos.gridX == 0) xuvector = 0;
        int yuvector = 1;
        if (player->pos.gridY-entity->pos.gridY < 0) yuvector = -1;
        else if (player->pos.gridY-entity->pos.gridY == 0) yuvector = 0;

        if (distancePosX(player->pos, entity->pos) <= 1 &&
            distancePosY(player->pos, entity->pos) <= 1){
            xuvector = 0;
            yuvector = 0;
            modifyPlayerHealth(player, -extra->damage);
            addEventMessage("Entity %s%s damaged you by %d\u0100", entity->name, entity->sprite, extra->damage);
        }

        entity->pos.gridX += xuvector;
        entity->pos.gridY += yuvector;

        if (extra->followPlayer == 0){
            addEventMessage("%s%s turns away, no longer interested in you", entity->name, entity->sprite);
        }
    }
    else{
        extra->followPlayer = extra->maxfp;
    }
}

void SnakeOnAction(Entity* entity){
    SnakeExtra* extra = (SnakeExtra *) entity->EntityExtra;
    Player* player = getPlayerInstance();

    if (findPlayerRoom() == (Room *) findEntityRoom(entity)){
        int xuvector = 1;
        if (player->pos.gridX-entity->pos.gridX < 0) xuvector = -1;
        else if (player->pos.gridX-entity->pos.gridX == 0) xuvector = 0;
        int yuvector = 1;
        if (player->pos.gridY-entity->pos.gridY < 0) yuvector = -1;
        else if (player->pos.gridY-entity->pos.gridY == 0) yuvector = 0;

        if (distancePosX(player->pos, entity->pos) <= 1 &&
            distancePosY(player->pos, entity->pos) <= 1){
            xuvector = 0;
            yuvector = 0;
            modifyPlayerHealth(player, -extra->damage);
            addEventMessage("Entity %s%s damaged you by %d\u0100", entity->name, entity->sprite, extra->damage);
        }

        entity->pos.gridX += xuvector;
        entity->pos.gridY += yuvector;
    }
}

void UndeadOnAction(Entity* entity){
    UndeadExtra* extra = (UndeadExtra *) entity->EntityExtra;
    Player* player = getPlayerInstance();

    if (distancePosX(player->pos, entity->pos) <= 1 &&
        distancePosY(player->pos, entity->pos) <= 1){
        extra->triggered = 1;
        addEventMessage("Entity %s%s is angry!", entity->name, entity->sprite);
    }

    if (findPlayerRoom() == (Room *) findEntityRoom(entity)){
        if (!extra->triggered) return;
        if (extra->followPlayer <= 0){
            return;
        }
        else{
            extra->followPlayer--;
        }
        int xuvector = 1;
        if (player->pos.gridX-entity->pos.gridX < 0) xuvector = -1;
        else if (player->pos.gridX-entity->pos.gridX == 0) xuvector = 0;
        int yuvector = 1;
        if (player->pos.gridY-entity->pos.gridY < 0) yuvector = -1;
        else if (player->pos.gridY-entity->pos.gridY == 0) yuvector = 0;

        if (distancePosX(player->pos, entity->pos) <= 1 &&
            distancePosY(player->pos, entity->pos) <= 1){
            xuvector = 0;
            yuvector = 0;
            modifyPlayerHealth(player, -extra->damage);
            addEventMessage("Entity %s%s damaged you by %d\u0100", entity->name, entity->sprite, extra->damage);
        }

        entity->pos.gridX += xuvector;
        entity->pos.gridY += yuvector;

        if (extra->followPlayer == 0){
            extra->triggered = 0;
            extra->followPlayer = extra->maxfp;
            addEventMessage("%s%s turns away, no longer interested in you", entity->name, entity->sprite);
        }
    }
    else{
        extra->followPlayer = extra->maxfp;
    }
}


void EntityOnAction(Entity* entity){
    switch(entity->entityclass){
    case EC_DEMON:{
        DemonOnAction(entity);
        break;
    }
    case EC_DRAGON:{
        DragonOnAction(entity);
        break;
    }
    case EC_GIANT:{
        GiantOnAction(entity);
        break;
    }
    case EC_SNAKE:{
        SnakeOnAction(entity);
        break;
    }
    case EC_UNDEAD:{
        UndeadOnAction(entity);
        break;
    }
    }

}


