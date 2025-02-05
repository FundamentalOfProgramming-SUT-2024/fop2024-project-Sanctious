#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include "map.h"

typedef struct {
    int hungerDrainTicks;
    int healthRegenCDT; // Health regen cooldown ticks
    int maxArmor;
    int maxHealth;
    int maxHunger;
    int starveThreshold;

} Difficulty;

typedef enum {
    MS_NORMAL,
    MS_LARGE,
    MS_GAMER,
    MS_SIGMABOI
} MapSize;

typedef struct{
    char savename[MAX_STR_SIZE];

    long long playtime;
    long long gold;
    int gametime;
    int numFloors;
    int curFloor;

    Difficulty difficulty;
    Color playerSkin;
    MapSize mapsize;

    int gameFinished;
} SaveInfo;

SaveInfo* getCurrentSave();
void setCurrentSave(SaveInfo* saveinfo);


void createSave(SaveInfo* saveinfo);
void loadGame(char* name);
void saveGame();
int saveExists(char* name);

#endif
