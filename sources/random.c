#include "random.h"
#include "logger.h"
#include "stdlib.h"
#include "time.h"


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
