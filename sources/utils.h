#ifndef RANDOM_H
#define RANDOM_H

#include "gc.h"

void initRandom();
int randomRange(int x, int y);
int weightedRandom(int x, int y, int weights[]);
int clamp(int value, int min, int max);
int comparePos(gCord pos1, gCord pos2);
int distancePos(gCord pos1, gCord pos2);
int distancePosX(gCord pos1, gCord pos2);
int distancePosY(gCord pos1, gCord pos2);
void formatTime(int secs, int *hours, int *minutes, int *seconds);

#endif
