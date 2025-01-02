#include "scene.h"
#include <stdlib.h>
#include <strings.h>
#include "logger.h"
#include "main.h"

void addScene(char* name, void (*renderer)()){
    Scene* scene = (Scene *) malloc(1 * sizeof(Scene));

    scene->sceneID = (char *) malloc(MAX_STR_SIZE * sizeof(char));
    strcpy(scene->sceneID, name);

    scene->renderfunc = renderer;

    Game* game = getGameInstance();

    game->scenes[game->num_scenes] = scene;
    game->num_scenes++;
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
