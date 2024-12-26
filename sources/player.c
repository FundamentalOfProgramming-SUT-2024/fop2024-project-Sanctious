#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "player.h"

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

    Log("Player initialized successfully.", _DEBUG_);
    Log("Player GOLD: %d", _DEBUG_, player->gold);
}
