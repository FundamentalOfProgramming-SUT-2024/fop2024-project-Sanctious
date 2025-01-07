#ifndef MAIN_H
#define MAIN_H

#include "scene.h"
#include "config.h"
#include "auth.h"

typedef struct {
    User* curUser;

    int num_scenes;
    Scene* scenes[MAX_NUM_SCENES];
    Scene* currentScene;
} Game;

Game* getGameInstance();

#endif
