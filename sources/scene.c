#include "scene.h"
#include "renderlibs.h"
#include <stdlib.h>
#include <strings.h>
#include "logger.h"
#include "main.h"
#include "scene.h"

void addScene(Scene* scene){
    Game* game = getGameInstance();

    game->scenes[game->num_scenes] = scene;
    game->num_scenes++;

    Log("Scene: %s created successfully.", _DEBUG_, scene->sceneID);
}

void changeScene(Scene* nextScene){
    Game* game = getGameInstance();
    Scene* currentScene = game->currentScene;

    if (currentScene->onExit != NULL){
        (*currentScene->onExit)();
    }
    if (nextScene->onEnter != NULL){
        (*nextScene->onEnter)();
    }

    // Null reference should be included
    glutKeyboardFunc(*nextScene->onKeypress);
    glutSpecialFunc(*nextScene->onSpecialKeypress);

//    glutIdleFunc(*nextScene->update); // FPS Limit
    glutDisplayFunc(*nextScene->update);

    Log("Changed scene from %s to %s.", _DEBUG_, currentScene->sceneID, nextScene->sceneID);
    game->currentScene = nextScene;
}

Scene* getSceneByID(char* name){
    Game* game = getGameInstance();
    for (int i = 0; i < game->num_scenes; i++){
        if (strcmp(game->scenes[i]->sceneID, name) == 0){
            return game->scenes[i];
        }
    }

    return NULL;
}
