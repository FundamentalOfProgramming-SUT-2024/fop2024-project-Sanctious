#include "map.h"
#include "structures.h"
#include "logger.h"
#include <stdlib.h>

Structure* generateBaseStructure(char sprite, Color spriteColor, gCord pos){
    Structure* structure = (Structure *) malloc(1 * sizeof(Structure));
    structure->sprite = sprite;
    structure->spriteColor = spriteColor;
    structure->pos = pos;

    return structure;
}

Structure* generateTrap(Structure* baseStructure, int damage){
    baseStructure->type = ST_TRAP;

    TrapExtra* extra = (TrapExtra *) malloc(1 * sizeof(TrapExtra));

    extra->damage = 5;

    baseStructure->StructureExtra = (void *) extra;

    Log("Trap generated with pos: (%d, %d) damage: %d.", _DEBUG_,
             baseStructure->pos.gridX, baseStructure->pos.gridY, extra->damage);
    return baseStructure;
}
