#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"
#include "map.h"
#include "item.h"

#define RANDOM(min, max) ((rand() % ((max) - (min) + 1)) + (min))

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
    srand(time(NULL));

    map->num_rooms = 0;
    map->gridWidth = 200;
    map->gridHeight = 200;

    int _tlwidth = 0;
    int _tlheight = 0;
    int _thwidth = 200;
    int _thheight = 200;

    for (int i = 0; i < 5; i++){
        // Or create n rooms beforehand then loop through them
        Room* _room = (Room *) malloc(1 * sizeof(Room));
        _room->type = TREASURE;

        getMapInstance()->rooms[getMapInstance()->num_rooms] = _room;
        getMapInstance()->num_rooms++;

        // split vertical
        if (rand()%2){
            _room->gridXPosition = RANDOM(_tlwidth, _thwidth);
            _room->gridYPosition = RANDOM(_tlheight, _thheight);
            _room->gridHeight = 0;
            _room->gridWidth = 0;


            _tlheight = (_thheight+_tlheight)/2;
        }
        // split vertical
        else{
            _room->gridXPosition = RANDOM(_tlwidth, _thwidth);
            _room->gridYPosition = RANDOM(_tlheight, _thheight);
            _room->gridHeight = 0;
            _room->gridWidth = 0;

            _tlwidth = (_thwidth+_tlwidth)/2;
        }

        Log("Room number %d created with gx:%d gy:%d gsx:%d gsy:%d.", DEBUG_, i+1,
            _room->gridXPosition, _room->gridYPosition, _room->gridWidth, _room->gridHeight);
    }

    Log("Map initialized successfully.", DEBUG_);
}

void renderRoom(Room* room){

}

