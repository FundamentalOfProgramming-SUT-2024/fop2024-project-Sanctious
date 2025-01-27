#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "player.h"
#include "map.h"

void initializePlayer();
static Player* instance = NULL;

// Singleton design
void setPlayerInstance(Player* player){
    instance = player;
}

Player* getPlayerInstance() {
    // First call
    if (instance == NULL) {
        instance = (Player*)malloc(sizeof(Player));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", _ERROR_, __FILE__, __LINE__);
            exit(1);
        }
        initializePlayer(instance);
    }
    return instance;
}


void initializePlayer(Player* player){
    player->gold = 2;
    player->inventory_size = 0;

    player->pos.gridX = getFloor(getCurFloor())->rooms[0]->pos.gridX + 1;
    player->pos.gridY = getFloor(getCurFloor())->rooms[0]->pos.gridY + 1;

    Log("Player initialized successfully.", _DEBUG_);
    Log("Player GOLD: %d", _DEBUG_, player->gold);
}

Room* findPlayerRoom(){
    Map* map = getFloor(getCurFloor());
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
    // Only calculate this once and you are good to go
    Map* map = getFloor(getCurFloor());
    for (int i = 0; i < map->num_corridors; i++){
        Corridor* cor = map->corridors[i];
        for (int j = 0; j < cor->path_length; j++){
            if (gridXP == cor->path[j].gridX && gridYP == cor->path[j].gridY) return 1;
        }
    }
    for (int i = 0; i < map->num_rooms; i++) {
        Room* room = map->rooms[i];
        if (room->pos.gridX <= gridXP &&
                gridXP < room->scale.gridW+room->pos.gridX &&
                room->pos.gridY <= gridYP &&
                gridYP < room->scale.gridH+room->pos.gridY)
            return 1;
    }
    return 0;
}


