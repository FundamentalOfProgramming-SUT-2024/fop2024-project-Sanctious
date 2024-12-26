#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"
#include "map.h"
#include "item.h"
#include "config.h"


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
            Log("MEM alloc failed f:%s l:%d", _ERROR_, __FILE__, __LINE__);
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
    map->scale.gridW = XCELLS;
    map->scale.gridH = YCELLS;

    int _tlwidth = 0;
    int _tlheight = 0;
    int _thwidth = XCELLS-1;
    int _thheight = YCELLS-1;

    Room* _room = (Room *) malloc(1 * sizeof(Room));
    _room->type = TREASURE;

    getMapInstance()->rooms[getMapInstance()->num_rooms] = _room;
    getMapInstance()->num_rooms++;

    _room->pos.gridX = _thwidth/2-3;
    _room->pos.gridY = _thheight/2-3;
    _room->scale.gridH = 10;
    _room->scale.gridW = 10;


    Log("Room number %d created with gx:%d gy:%d gsx:%d gsy:%d.", _DEBUG_, 1,
        _room->pos.gridX, _room->pos.gridY, _room->scale.gridW, _room->scale.gridH);

    // generate map
//    for (int i = 0; i < 3; i++){
//        // Or create n rooms beforehand then loop through them
//        Room* _room = (Room *) malloc(1 * sizeof(Room));
//        _room->type = TREASURE;
//
//        getMapInstance()->rooms[getMapInstance()->num_rooms] = _room;
//        getMapInstance()->num_rooms++;
//
//        // split vertical
//        if (rand()%2){
//            _room->pos.gridX = RANDOM(_tlwidth, _thwidth);
//            _room->pos.gridY = RANDOM(_tlheight, _thheight);
//            _room->scale.gridH = 1;
//            _room->scale.gridW = 1;
//
//
//            _tlheight = (_thheight+_tlheight)/2;
//        }
//        // split vertical
//        else{
//            _room->pos.gridX = RANDOM(_tlwidth, _thwidth);
//            _room->pos.gridY = RANDOM(_tlheight, _thheight);
//            _room->scale.gridH = 1;
//            _room->scale.gridW = 1;
//
//            _tlwidth = (_thwidth+_tlwidth)/2;
//        }
//
//        Log("Room number %d created with gx:%d gy:%d gsx:%d gsy:%d.", _DEBUG_, i+1,
//            _room->pos.gridX, _room->pos.gridY, _room->scale.gridW, _room->scale.gridH);
//    }

    Log("Map initialized successfully.", _DEBUG_);
}


