#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "map.h"

void initializeMap();
static Map* instance = NULL;

// Singleton design

Map* getMapInstance() {
    // First call
    if (instance == NULL) {
        instance = (Map*)malloc(sizeof(Map));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", ERROR, __FILE__, __LINE__);
            exit(1);
        }
        initializeMap();
        // implemenet

    }
    return instance;
}


void initializeMap(){

    Log("Map initialized successfully.", INFO);
}

