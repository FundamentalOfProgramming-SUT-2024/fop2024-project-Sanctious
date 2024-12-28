#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "logger.h"
#include "map.h"
#include "item.h"
#include "config.h"


#define RANDOM(min, max) ((rand() % ((max) - (min) + 1)) + (min)) // Inclusive
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

void generateRoom(Map* map, int gx, int gy, int gw, int gh, RoomType type){
    Room* _room = (Room *) malloc(1 * sizeof(Room));
    _room->type = type;
    _room->num_doors = 0;
    _room->num_items = 0;

    map->rooms[map->num_rooms] = _room;
    map->num_rooms++;

    _room->pos.gridX = gx;
    _room->pos.gridY = gy;
    _room->scale.gridW = gw;
    _room->scale.gridH = gh;

    Log("Room number %d created with gx:%d gy:%d gsx:%d gsy:%d.", _DEBUG_, map->num_rooms,
        _room->pos.gridX, _room->pos.gridY, _room->scale.gridW, _room->scale.gridH);
}

void generateRooms(Map* map){

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


            int _x = RANDOM(_lowerXpos, _higherXpos);
            int _y = RANDOM(_lowerYpos, _higherYpos);

            // assuming that rooms dont collide
            // it is ensured that _higherXpos - _x is greater than MIN_ROOM_WIDTH
            generateRoom(map, _x, _y,
                RANDOM(MIN_ROOM_WIDTH, _higherXpos - _x + MIN_ROOM_WIDTH),
                RANDOM(MIN_ROOM_HEIGHT, _higherYpos - _y + MIN_ROOM_HEIGHT),
                TREASURE);




        }
    }

    Log("Map initialized successfully.", _DEBUG_);
}
void generateDoor(){}

void generateCorridor(Door* door){
    gCord pos1 = door->pos;
    gCord pos2 = door->connectedd->pos;

    // maybe implement valid position for corridor so that it doesnt run
    // into rooms and stuff like that

    // x unit vector
    int xuvector = 1;
    if (pos2.gridX-pos1.gridX < 0) xuvector = -1;
    // y unit vector
    int yuvector = 1;
    if (pos2.gridY-pos1.gridY < 0) yuvector = -1;
    int xprog = 0, yprog = 0;

    // Create a new corridor
    Corridor* corridor = (Corridor *) malloc(1 * sizeof(Corridor));
    corridor->path_length = abs(pos2.gridY-pos1.gridY) + abs(pos2.gridX-pos1.gridX) + 1;

    // for now path length is constant there is no collision with other rooms
    // Add current room pos to the list
    corridor->path[0].gridX = pos1.gridX;
    corridor->path[0].gridY = pos1.gridY;

    for (int i = 1; i <= corridor->path_length; i++){
        printf("%d %d %d %d\n", xprog, yprog, corridor->path_length);
        int random = rand()%2;
        if (xprog == pos2.gridX-pos1.gridX) random = 0;
        if (yprog == pos2.gridY-pos1.gridY) random = 1;

        if (random) xprog += xuvector;
        else yprog += yuvector;

        corridor->path[i].gridX = pos1.gridX + xprog;
        corridor->path[i].gridY = pos1.gridY + yprog;
    }


    door->corridor = corridor;
    door->connectedd->corridor = corridor;


}

void generateDoors(Map* map){
    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];
        int _x = RANDOM(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
        Door* door = (Door *) malloc(1 * sizeof(Door));

        door->parentr = room;
        door->pos.gridX = _x;
        door->pos.gridY = room->pos.gridY-1;
        room->doors[room->num_doors] = door;
        room->num_doors++;

    }
    map->rooms[0]->doors[0]->connectedd = map->rooms[1]->doors[0];
    generateCorridor(map->rooms[0]->doors[0]);
    printf("%d", map->rooms[0]->doors[0]->corridor->path_length);
//    for (int i = 0; i < )

}


void initializeMap(Map* map){
    srand(time(NULL));

    map->num_rooms = 0;
    map->scale.gridW = XCELLS;
    map->scale.gridH = YCELLS;

    generateRooms(map);
    generateDoors(map);

}


