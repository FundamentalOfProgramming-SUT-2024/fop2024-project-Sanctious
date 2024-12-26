#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "player.h"
#include "map.h"

void initializePlayer();
static Player* instance = NULL;

// Singleton design

Player* getPlayerInstance() {
    // First call
    if (instance == NULL) {
        instance = (Player*)malloc(sizeof(Player));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", _ERROR_, __FILE__, __LINE__);
            exit(1);
        }
        initializePlayer(instance);
        // implemenet

    }
    return instance;
}


void initializePlayer(Player* player){
    player->gold = 2;
    player->gridXPosition = 17;
    player->gridYPosition = 17;

    Log("Player initialized successfully.", _DEBUG_);
    Log("Player GOLD: %d", _DEBUG_, player->gold);
}

Room* findPlayerRoom(){
    Map* map = getMapInstance();
    Player* player = getPlayerInstance();

    for (int i = 0; i < map->num_rooms; i++){
        if (map->rooms[i]->gridXPosition <= player->gridXPosition &&
            player->gridXPosition < map->rooms[i]->gridWidth+map->rooms[i]->gridXPosition &&
            map->rooms[i]->gridYPosition <= player->gridYPosition &&
            player->gridYPosition < map->rooms[i]->gridHeight+map->rooms[i]->gridYPosition)

            return map->rooms[i];
    }
    return NULL;
}

char isValidPos(int gridXP, int gridYP){
    // A bad implementation
    Room* room = findPlayerRoom();
    if (room->gridXPosition <= gridXP &&
            gridXP < room->gridWidth+room->gridXPosition &&
            room->gridYPosition <= gridYP &&
            gridYP < room->gridHeight+room->gridYPosition)
        return 1;
    return 0;
}
