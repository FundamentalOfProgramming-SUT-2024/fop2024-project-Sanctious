#ifndef MAIN_H
#define MAIN_H

#include "map.h"

typedef struct{
    char savename[MAX_STR_SIZE];

    long long playtime;
    long long gold;

    Difficulty difficulty;
    MapSize mapsize;

} SaveInfo;

SaveInfo* getCurrentSave();
void setCurrentSave(SaveInfo* saveinfo);

#endif
