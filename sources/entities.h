#ifndef ENTITIES_H
#define ENTITIES_H

#include "gc.h"
#include "config.h"

typedef enum{
    EC_DEMON,
    EC_GIANT,
    EC_SNAKE,
    EC_DRAGON,
    EC_UNDEED
} EntityClass;


typedef struct{
    int a;
} DemonExtra;

typedef struct{
    int a;
} GiantExtra;

typedef struct{
    int a;
} SnakeExtra;

typedef struct{
    int a;
} DragonExtra;

typedef struct{
    int a;
} UndeedExtra;

typedef struct {
    char name[MAX_STR_SIZE];
    char sprite;
    gCord pos;

    int armor;
    int goldDrop;
    int health;

    EntityClass entityclass;
    void* EntityExtra;

} Entity;

Entity* createEntity(char* name, gCord pos, char sprite);
Entity* createDemon(Entity* entity);

#endif

