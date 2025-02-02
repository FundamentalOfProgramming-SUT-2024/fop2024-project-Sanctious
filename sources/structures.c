#include <stdlib.h>
#include "map.h"
#include "structures.h"
#include "logger.h"
#include "strings.h"
#include "player.h"
#include "scenes/s_game.h"

Structure* generateBaseStructure(char sprite[5], Color spriteColor, gCord pos){
    Structure* structure = (Structure *) malloc(1 * sizeof(Structure));
    strcpy(structure->sprite, sprite);
    structure->spriteColor = spriteColor;
    structure->pos = pos;

    return structure;
}

Structure* generateTrap(Structure* structure, int damage){
    structure->type = ST_TRAP;

    TrapExtra* extra = (TrapExtra *) malloc(1 * sizeof(TrapExtra));

    extra->damage = 5;

    structure->StructureExtra = (void *) extra;

    Log("Trap generated with pos: (%d, %d) damage: %d.", _DEBUG_,
             structure->pos.gridX, structure->pos.gridY, extra->damage);
    return structure;
}

Structure* generateStairs(Structure* structure, int floorIndex){
    structure->type = ST_STAIRS;

    StairsExtra* extra = (StairsExtra *) malloc(1 * sizeof(StairsExtra));

    extra->floorIndex = floorIndex;
    extra->prevPos = (gCord) {-1, -1};
    extra->nextPos = (gCord) {-1, -1};

    structure->StructureExtra = (void *) extra;

    Log("Stairs generated with pos: (%d, %d) floor: %d.", _DEBUG_,
             structure->pos.gridX, structure->pos.gridY, extra->floorIndex+1);
    return structure;
}

int TrapOnStep(Structure* structure){
    TrapExtra* extra = (TrapExtra *) structure->StructureExtra;
    Player* player = getPlayerInstance();

    modifyPlayerHealth(player, -extra->damage);

    addEventMessage("You stepped on a trap %d\u010c", extra->damage);
    return 1;
}


int StructureOnStep(Structure* structure){
    switch(structure->type){
    case ST_STAIRS:

        break;
    case ST_TRAP:
        return TrapOnStep(structure);
        break;
    }
    return 0;
}
