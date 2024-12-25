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


//    for (int i = 0; i < map->num_rooms; i++){
//        printf("%d\n", map->rooms[i]->gridXPosition);
//    }
    // main loop
    // render rooms/map
    // render player/other objects



    return 0;
}

