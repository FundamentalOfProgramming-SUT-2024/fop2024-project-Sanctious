#include "utils.h"
#include "logger.h"
#include "stdlib.h"
#include "time.h"
#include "gc.h"

void initRandom(){
    srand(time(NULL));
}

int random(int x, int y){
    return (rand() % (y - x + 1)) + x;
}

int weightedRandom(int x, int y, int weights[]){
    int totalWeight = 0;

    for (int i = 0; i < y-x+1; i++){
        totalWeight += weights[i];
    }

    int randomValue = rand() % totalWeight;
    int cumulativeWeight = 0;

    for (int i = 0; i < y-x+1; i++) {
        cumulativeWeight += weights[i];
        if (randomValue < cumulativeWeight) {
            return x + i;
        }
    }
}

int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

int comparePos(gCord pos1, gCord pos2){
    if (pos1.gridX != pos2.gridX ) return 0;
    if (pos1.gridY != pos2.gridY ) return 0;
    return 1;
}

int distancePos(gCord pos1, gCord pos2){
    return abs(pos1.gridX-pos2.gridX) + abs(pos1.gridY-pos2.gridY);
}
