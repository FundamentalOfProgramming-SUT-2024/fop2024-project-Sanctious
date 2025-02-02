#ifndef RANDOM_H
#define RANDOM_H

#include "gc.h"

void initRandom();
int random(int x, int y);
int weightedRandom(int x, int y, int weights[]);
int clamp(int value, int min, int max);
int comparePos(gCord pos1, gCord pos2);
int distancePos(gCord pos1, gCord pos2);

#endif
