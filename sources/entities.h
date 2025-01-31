#ifndef ENTITIES_H
#define ENTITIES_H

#include "gc.h"
#include "config.h"
#include "renderer.h"

typedef enum{
    EC_DEMON = 0,
    EC_GIANT = 1,
    EC_SNAKE = 2,
    EC_UNDEAD = 3,
    EC_DRAGON = 4,
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
} UndeadExtra;

typedef struct {
    char name[MAX_STR_SIZE];
    char sprite[5]; // Custom sprite support
    Color spriteColor;
    gCord pos;

    int armor;
    int goldDrop;
    int health;

    EntityClass entityclass;
    void* EntityExtra;

} Entity;

Entity* createEntity(char* name, gCord pos, char sprite[5], Color spriteColor);
Entity* createDemon(Entity* entity);
Entity* createSnake(Entity* entity);
Entity* createGiant(Entity* entity);
Entity* createDragon(Entity* entity);
Entity* createUndead(Entity* entity);
//Entity* createDragon(Entity* entity);
//Entity* createGiant(Entity* entity);
//Entity* createSnake(Entity* entity);
//Entity* createUndead(Entity* entity);
void EntityOnAction(Entity* entity);
void* findEntityRoom(Entity* entity);

#endif

