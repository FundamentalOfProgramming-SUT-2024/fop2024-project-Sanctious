#ifndef SCENE_H
#define SCENE_H


typedef struct{
    char* sceneID;

    void (*onKeypress)(unsigned char, int, int);
    void (*onSpecialKeypress)(int, int, int);
    void (*update)();
    void (*onEnter)();
    void (*onExit)();
} Scene;

// scenes are created on init and their settings are implemented in their own code
// every c file that starts with s_* is a scene
// char *, void (*)(), (*)(unsigned char, int, int), (*)(int, int, int)
void addScene(Scene*);
void changeScene(Scene*);

Scene* getSceneByID(char *);

#endif
