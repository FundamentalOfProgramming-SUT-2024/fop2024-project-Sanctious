#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include "map.h"

typedef struct{
    char savename[MAX_STR_SIZE];

    long long playtime;
    long long gold;
    int gametime;
    int numFloors;
    int curFloor;

    Difficulty difficulty;
    MapSize mapsize;

} SaveInfo;

SaveInfo* getCurrentSave();
void setCurrentSave(SaveInfo* saveinfo);


void createSave(SaveInfo* saveinfo);
void loadGame(char* name);
void saveGame();

#endif
