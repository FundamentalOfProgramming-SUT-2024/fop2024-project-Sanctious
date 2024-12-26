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
    player->pos.gridX = 17;
    player->pos.gridY = 17;

    Log("Player initialized successfully.", _DEBUG_);
    Log("Player GOLD: %d", _DEBUG_, player->gold);
}

Room* findPlayerRoom(){
    Map* map = getMapInstance();
    Player* player = getPlayerInstance();

    for (int i = 0; i < map->num_rooms; i++){
        if (map->rooms[i]->pos.gridX <= player->pos.gridX &&
            player->pos.gridX < map->rooms[i]->scale.gridW+map->rooms[i]->pos.gridX &&
            map->rooms[i]->pos.gridY <= player->pos.gridY &&
            player->pos.gridY < map->rooms[i]->scale.gridH+map->rooms[i]->pos.gridY)

            return map->rooms[i];
    }
    return NULL;
}

char isValidPos(int gridXP, int gridYP){
    // A bad implementation
    Room* room = findPlayerRoom();
    if (room->pos.gridX <= gridXP &&
            gridXP < room->scale.gridW+room->pos.gridX &&
            room->pos.gridY <= gridYP &&
            gridYP < room->scale.gridH+room->pos.gridY)
        return 1;
    return 0;
}
