#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "logger.h"
#include "player.h"
#include "map.h"


int main()
{
    Log("Program started.", DEBUG_);

    // Initialize Player and Map
    Player* mainPlayer = getPlayerInstance();
    Map* map = getMapInstance();


    Log("Player GOLD: %d", DEBUG_, mainPlayer->gold);




    return 0;
}

