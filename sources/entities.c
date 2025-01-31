#include "entities.h"
#include "stdlib.h"
#include "strings.h"
#include "logger.h"
#include "renderer.h"
#include "player.h"

extern const int odirs[8][2];

void* findEntityRoom(Entity* entity){
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

Entity* createEntity(char* name, gCord pos, char sprite[5], Color spriteColor){
    Entity* entity = (Entity *) malloc(1 * sizeof(Entity));

    strcpy(entity->name, name);
    strcpy(entity->sprite, sprite);
    entity->spriteColor = spriteColor;
    entity->pos = pos;

    return entity;
}

Entity* createDemon(Entity* entity){
    entity->entityclass = EC_DEMON;
    DemonExtra* extra = (DemonExtra *) malloc(1 * sizeof(DemonExtra));

//    extra->subclass = subclass;
//    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Demon created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createSnake(Entity* entity){
    entity->entityclass = EC_SNAKE;
    SnakeExtra* extra = (SnakeExtra *) malloc(1 * sizeof(SnakeExtra));

//    extra->subclass = subclass;
//    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Snake created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createGiant(Entity* entity){
    entity->entityclass = EC_GIANT;
    GiantExtra* extra = (GiantExtra *) malloc(1 * sizeof(GiantExtra));

//    extra->subclass = subclass;
//    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Giant created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createDragon(Entity* entity){
    entity->entityclass = EC_DRAGON;
    DragonExtra* extra = (DragonExtra *) malloc(1 * sizeof(DragonExtra));

//    extra->subclass = subclass;
//    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Dragon created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

Entity* createUndead(Entity* entity){
    entity->entityclass = EC_UNDEAD;
    UndeadExtra* extra = (UndeadExtra *) malloc(1 * sizeof(UndeadExtra));

//    extra->subclass = subclass;
//    extra->damage = damage;

    entity->EntityExtra = (void *) extra;

    Log("Undead created with sprite: %s", _DEBUG_, entity->sprite);
    return entity;
}

void DemonOnAction(Entity* entity){
    DemonExtra* extra = (DemonExtra *) entity->EntityExtra;
    return;
    Player* player = getPlayerInstance();
        if (findPlayerRoom() == (Room *) findEntityRoom(entity)){
        int xuvector = 1;
        if (player->pos.gridX-entity->pos.gridX < 0) xuvector = -1;
        else if (player->pos.gridX-entity->pos.gridX == 0) xuvector = 0;
        // y unit vector
        int yuvector = 1;
        if (player->pos.gridY-entity->pos.gridY < 0) yuvector = -1;
        else if (player->pos.gridY-entity->pos.gridY == 0) yuvector = 0;

        for (int i = 0; i < 8; i++){
            if (player->pos.gridY-entity->pos.gridY == odirs[i][1] &&
                player->pos.gridX-entity->pos.gridX == odirs[i][0]){
                xuvector = 0;
                yuvector = 0;
                break;
            }
        }
        entity->pos.gridX += xuvector;
        entity->pos.gridY += yuvector;
    }
}

void DragonOnAction(Entity* entity){


}

void GiantOnAction(Entity* entity){


}

void SnakeOnAction(Entity* entity){


}

void UndeadOnAction(Entity* entity){


}


void EntityOnAction(Entity* entity){
    switch(entity->entityclass){
    case EC_DEMON:{
        DemonOnAction(entity);
    }
    case EC_DRAGON:{
        DragonOnAction(entity);
    }
    case EC_GIANT:{
        GiantOnAction(entity);
    }
    case EC_SNAKE:{
        SnakeOnAction(entity);
    }
    case EC_UNDEAD:{
        UndeadOnAction(entity);
    }
    }

}
