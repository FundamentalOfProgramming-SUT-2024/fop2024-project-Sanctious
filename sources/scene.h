#ifndef SCENE_H
#define SCENE_H

#include "config.h"

struct Scene;
typedef struct Scene Scene;

struct Scene{
    char sceneID[MAX_STR_SIZE];
    // Used by scene manager
    Scene* parentScene;

    void (*onKeypress)(unsigned char, int, int);
    void (*onSpecialKeypress)(int, int, int);
    void (*update)();
    void (*onEnter)();
    void (*onExit)();
};

// scenes are created on init and their settings are implemented in their own code
// every c file that starts with s_* is a scene
// char *, void (*)(), (*)(unsigned char, int, int), (*)(int, int, int)
void addScene(Scene*);
void changeScene(Scene*);

Scene* getSceneByID(char *);

#endif
