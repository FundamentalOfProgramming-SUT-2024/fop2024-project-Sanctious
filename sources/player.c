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
            Log("MEM alloc failed f:%s l:%d", ERROR, __FILE__, __LINE__);
            exit(1);
        }
        initializePlayer();
        // implemenet

    }
    return instance;
}


void initializePlayer(){
    getPlayerInstance()->gold = 2;

    Log("Player initialized successfully.", INFO);
    Log("Player GOLD: %d", DEBUG_, getPlayerInstance()->gold);
}
