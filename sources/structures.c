#include "map.h"
#include "structures.h"
#include "logger.h"
#include <stdlib.h>

Structure* generateTrap(){
    Structure* trap = (Structure *) malloc(1 * sizeof(Structure));
    trap->sprite = '@';
    trap->type = ST_TRAP;

    TrapExtra* extra = (TrapExtra *) malloc(1 * sizeof(TrapExtra));

    extra->damage = 5;

    trap->StructureExtra = (void *) extra;

    return trap;
}
