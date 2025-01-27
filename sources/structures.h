#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "map.h"
#include "gc.h"

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
    void* StructureExtra;
    StructureType type;

} Structure;

Structure* generateTrap();

#endif
