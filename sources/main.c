#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "logger.h"
#include "player.h"
#include "map.h"
#include "item.h"


int main()
{
    Log("Program started.", DEBUG_);

    // Initialize Player and Map
    Map* map = getMapInstance();
    Player* mainPlayer = getPlayerInstance();


    // main loop
    // render rooms/map
    // render player/other objects



    return 0;
}

