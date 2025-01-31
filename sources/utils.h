#ifndef RANDOM_H
#define RANDOM_H

void initRandom();
int random(int x, int y);
int weightedRandom(int x, int y, int weights[]);
int clamp(int value, int min, int max);

#endif
