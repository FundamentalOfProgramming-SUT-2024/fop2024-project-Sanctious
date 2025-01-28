#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "gc.h"
#include "renderer.h"

typedef enum {
    ST_WINDOW,
    ST_OBSTACLE,
    ST_TRAP
} StructureType;


typedef struct{
    int damage;
} TrapExtra;

typedef struct{
    gCord pos;
    char sprite;
    Color spriteColor;

    StructureType type;
    void* StructureExtra;

} Structure;

Structure* generateBaseStructure(char sprite, Color spriteColor, gCord pos);
Structure* generateTrap(Structure* trap, int damage);

#endif
