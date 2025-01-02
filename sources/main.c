#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "logger.h"
#include "player.h"
#include "map.h"
#include "item.h"
#include "renderer.h"
#include "main.h"
#include "scene.h"

static Game* instance = NULL;

void initializeGame(Game* game){
    game->num_scenes = 0;
    game->currentScene = NULL;
    addScene("main_menu", &display);
    game->currentScene = getSceneByID("main_menu");

    Log("Game handler initialized successfully.", _DEBUG_);
}
// Singleton design

Game* getGameInstance() {
    // First call
    if (instance == NULL) {
        instance = (Game*) malloc(sizeof(Game));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", _ERROR_, __FILE__, __LINE__);
            exit(1);
        }
        initializeGame(instance);
    }
    return instance;
}

int main(int argc, char** argv)
{
    Log("Game started...", _DEBUG_);
    Game* game = getGameInstance();

    // Initialize Player and Map
    Map* map = getMapInstance();
    Player* mainPlayer = getPlayerInstance();

    render(argc, argv);

//    for (int i = 0; i < map->num_rooms; i++){
//        printf("%d\n", map->rooms[i]->gridXPosition);
//    }
    // main loop
    // render rooms/map
    // render player/other objects



    return 0;
}

