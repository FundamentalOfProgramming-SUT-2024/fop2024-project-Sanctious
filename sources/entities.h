#ifndef ENTITIES_H
#define ENTITIES_H

#include "gc.h"
#include "config.h"
#include "renderer.h"

typedef struct Room Room;

typedef enum{
    EC_DEMON = 0,
    EC_GIANT = 1,
    EC_SNAKE = 2,
    EC_UNDEAD = 3,
    EC_DRAGON = 4,
} EntityClass;

typedef struct{
    int damage;
} DemonExtra;

typedef struct{
    int damage;
} GiantExtra;

typedef struct{
    int damage;
} SnakeExtra;

typedef struct{
    int damage;
} DragonExtra;

typedef struct{
    int damage;
} UndeadExtra;

typedef struct {
    char name[MAX_STR_SIZE];
    char sprite[5]; // Custom sprite support
    Color spriteColor;
    gCord pos;

    int goldDrop;
    int armor;
    int maxArmor;
    int health;
    int maxHealth;

    EntityClass entityclass;
    void* EntityExtra;

} Entity;

Entity* createEntity(char* name, gCord pos, char sprite[5], Color spriteColor);
Entity* createDemon(Entity* entity, int damage);
Entity* createSnake(Entity* entity, int damage);
Entity* createGiant(Entity* entity, int damage);
Entity* createDragon(Entity* entity, int damage);
Entity* createUndead(Entity* entity, int damage);
//Entity* createDragon(Entity* entity);
//Entity* createGiant(Entity* entity);
//Entity* createSnake(Entity* entity);
//Entity* createUndead(Entity* entity);
void EntityOnAction(Entity* entity);
Room* findEntityRoom(Entity* entity);

#endif

