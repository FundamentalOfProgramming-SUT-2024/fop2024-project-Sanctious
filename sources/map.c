#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "map.h"
#include "item.h"

void initializeMap();
static Map* instance = NULL;
// It shouldnt be here
void renderRoom(Room* room);

// Singleton design

Map* getMapInstance() {
    // First call
    if (instance == NULL) {
        instance = (Map*)malloc(sizeof(Map));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", ERROR, __FILE__, __LINE__);
            exit(1);
        }
        initializeMap(instance);
        // implemenet

    }
    return instance;
}


void initializeMap(Map* map){
    map->num_rooms = 0;
    map->gridWidth = 200;
    map->girdHeight = 200;


    for (int i = 0; i < 5; i++){
        Room* _room = (Room *) malloc(1 * sizeof(Room));
        _room->gridXPosition = 0;
        _room->gridYPosition = 0;
        _room->gridHeight = 0;
        _room->gridWidth = 0;
        _room->type = TREASURE;

        getMapInstance()->rooms[getMapInstance()->num_rooms] = _room;
        getMapInstance()->num_rooms++;
        Log("Room number %d created.", DEBUG_, i+1);
    }

    Log("Map initialized successfully.", DEBUG_);
}

void renderRoom(Room* room){

}

