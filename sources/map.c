#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"
#include "map.h"
#include "item.h"
#include "config.h"


#define RANDOM(min, max) ((rand() % ((max) - (min) + 1)) + (min))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

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

    int _lowerXpos, _higherXpos, _lowerYpos, _higherYpos;

    // i should replace this with a more dynamic code
    // maybe other variable names
    // adding buffer zone to one pos is enough
    // Generate map

    for (int i = 0; i < MAPDIV; i++){
        _lowerXpos = (XCELLS-1)/MAPDIV * i + BUFFER_ZONE;
        _higherXpos = (XCELLS-1)/MAPDIV * (i+1) -MIN_ROOM_WIDTH;
        for (int j = 0; j < MAPDIV; j++){
            _lowerYpos = (YCELLS-1)/MAPDIV * j + BUFFER_ZONE;
            _higherYpos = (YCELLS-1)/MAPDIV * (j+1) -MIN_ROOM_HEIGHT;
            // assuming that rooms dont collide
            Room* _room = (Room *) malloc(1 * sizeof(Room));
            _room->type = TREASURE;

            getMapInstance()->rooms[getMapInstance()->num_rooms] = _room;
            getMapInstance()->num_rooms++;

            int roomminw = MIN_ROOM_WIDTH;
            int roomminh = MIN_ROOM_HEIGHT;

            _room->pos.gridX = RANDOM(_lowerXpos, _higherXpos);
            _room->pos.gridY = RANDOM(_lowerYpos, _higherYpos);

            int roommaxw = MAX(roomminw, _higherXpos - _room->pos.gridX);
            int roommaxh = MAX(roomminh, _higherYpos - _room->pos.gridY);

            _room->scale.gridH = RANDOM(roomminh, roommaxh);
            _room->scale.gridW = RANDOM(roomminw, roommaxw);


            Log("Room number %d created with gx:%d gy:%d gsx:%d gsy:%d.", _DEBUG_, getMapInstance()->num_rooms,
                _room->pos.gridX, _room->pos.gridY, _room->scale.gridW, _room->scale.gridH);
        }
    }

    Log("Map initialized successfully.", _DEBUG_);
}


