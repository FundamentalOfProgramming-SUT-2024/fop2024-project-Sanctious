#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "logger.h"
#include "player.h"
#include "map.h"
#include "item.h"
#include "renderer.h"
#include "renderlibs.h"
#include "main.h"
#include "scene.h"

// ***********************************************************
extern void initscene_main_menu();
extern void initscene_game();
extern void initscene_leaderboard_menu();
extern void initscene_loadgame_menu();
extern void initscene_newgame_menu();
extern void initscene_profile_menu();
extern void initscene_register_menu();
extern void initscene_login_menu();
extern void initscene_settings_menu();
extern void initscene_authentication_menu();

const int NUM_SCENES = 10;
const void (*SCENES[MAX_SCENES])() =
    {   &initscene_main_menu,
        &initscene_game,
        &initscene_leaderboard_menu,
        &initscene_loadgame_menu,
        &initscene_newgame_menu,
        &initscene_profile_menu,
        &initscene_register_menu,
        &initscene_login_menu,
        &initscene_settings_menu,
        &initscene_authentication_menu,
    };
// ***********************************************************


static Game* instance = NULL;

void initializeGame(Game* game){
    game->num_scenes = 0; // It's handled in addScene function
    game->currentScene = NULL;

    for (int i = 0; i < NUM_SCENES; i++){
        (*(SCENES[i]))();
    }

    game->currentScene = getSceneByID("game");

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

    // Initialize Player and Map
    Map* map = getMapInstance();
    Player* mainPlayer = getPlayerInstance();

    initializeRenderer(argc, argv);

    // Initialize Game and create Scenes
    Game* game = getGameInstance();

    Log("Entering game loop...", _DEBUG_);
    glutMainLoop();

    return 0;
}

