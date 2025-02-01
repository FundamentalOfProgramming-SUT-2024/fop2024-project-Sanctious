#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "gc.h"
#include "renderer.h"

typedef enum {
    ST_WINDOW,
    ST_OBSTACLE,
    ST_TRAP,
    ST_STAIRS,
} StructureType;


typedef struct{
    int damage;
} TrapExtra;

typedef struct{
    int floorIndex;
    gCord prevPos;
    gCord nextPos;
} StairsExtra;

typedef struct{
    gCord pos;
    char sprite[5]; // Custom sprite support
    Color spriteColor;

    StructureType type;
    void* StructureExtra;

} Structure;

Structure* generateBaseStructure(char sprite[5], Color spriteColor, gCord pos);
Structure* generateTrap(Structure* structure, int damage);
Structure* generateStairs(Structure* structure, int floorIndex);

#endif
