#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "logger.h"



typedef struct {
    int data;
} Player;

void initlizeMap(){
    Log("Map initialized successfully.", INFO);
}

// Singleton design
typedef struct {
    int data;
} Map;

static Map* instance = NULL;

Map* getMapInstance() {
    // First call
    if (instance == NULL) {
        instance = (Map*)malloc(sizeof(Map));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", ERROR, __FILE__, __LINE__);
            exit(1);
        }
        initlizeMap();
        // implemenet

    }
    return instance;
}

int main()
{
    Map* map = getMapInstance();
//    initilizeMap()



    return 0;
}

