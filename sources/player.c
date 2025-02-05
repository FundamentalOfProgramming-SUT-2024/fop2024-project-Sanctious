#include <stdio.h>
#include <stdlib.h>

#include "string.h"
#include "logger.h"
#include "player.h"
#include "utils.h"
#include "item.h"
#include "map.h"
#include "savesystem.h"

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
    SaveInfo* saveinfo = getCurrentSave();

    strcpy(player->sprite, "\u00f0");
    player->spriteColor = saveinfo->playerSkin;

    player->gold = 0;
    player->maxHealth = saveinfo->difficulty.maxHealth;
    player->health = player->maxHealth;
    player->maxArmor = saveinfo->difficulty.maxArmor;
    player->armor = player->maxArmor;
    player->maxHunger = saveinfo->difficulty.maxHunger;
    player->hunger = player->maxHunger;

    for (int i = 0; i < 3; i++){
        player->multies[i] = 1;
        player->multiesT[i] = 0;
        player->def_multies[i] = 1;
    }

    player->combatTagTime = 0;
    player->inventory_size = 0;
    player->equippedItem = NULL;

    Room* room;
    do{
        room = getRandomRoom(getFloor(getCurFloor()));
    }
    while (getStairsInRoom(room) != NULL);

    player->pos = getRandomCordInRoom(room);

    Log("Player initialized successfully.", _DEBUG_);
    Log("Player Gold: %d", _DEBUG_, player->gold);
    Log("Player Pos: (%d, %d)", _DEBUG_, player->pos.gridX, player->pos.gridY);
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

Corridor* findPlayerCorridor(){
    Map* map = getFloor(getCurFloor());
    Player* player = getPlayerInstance();

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* corridor = map->corridors[i];
        for (int j = 0; j < corridor->path_length; j++){
            if (comparePos(player->pos, corridor->path[j])){
                return corridor;
            }
        }
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

void removeItemFromPlayer(Player* player, int itemIndex){
    for (int i = itemIndex; i < player->inventory_size-1; i++){
        player->inventory[i] = player->inventory[i+1];
    }
    player->inventory_size--;
}

void addItemToPlayer(Player* player, Item* item){
    player->inventory[player->inventory_size++] = item;
}

void modifyPlayerHealth(Player* player, int offset){
    player->health = clamp(player->health+offset, 0, player->maxHealth);
}

void modifyPlayerArmor(Player* player, int offset){
    player->armor = clamp(player->armor+offset, 0, player->maxArmor);
}

void modifyPlayerHunger(Player* player, int offset){
    player->hunger = clamp(player->hunger+offset, 0, player->maxHunger);
}
