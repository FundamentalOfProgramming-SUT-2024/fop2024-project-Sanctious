#ifndef SCENE_H
#define SCENE_H

void addScene(char *, void (*)());

typedef struct{
    char* sceneID;
    void (*renderfunc)();
} Scene;

Scene* getSceneByID(char *);

#endif
