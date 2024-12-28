#ifndef MAP_H
#define MAP_H

#include "item.h"
#include "config.h"

struct Room;
typedef struct Room Room;

struct Door;
typedef struct Door Door;

typedef enum {
    TREASURE,
    NORMAL,
    TRAP
} RoomType;

typedef struct {
    int gridX;
    int gridY;

} gCord;

typedef struct {
    int gridW;
    int gridH;

} gScale;

typedef struct {
    int path_length;
    gCord path[MAX_CORRIDOR_LENGTH];
} Corridor;

struct Door{

    gCord pos; // pos in parent room

    Corridor* corridor;
    Room* parentr; // parent room
    Door* connectedd; // connected door
} ;

struct Room{
    gCord pos;
    gScale scale;

    RoomType type;

    int num_items;
    Item* items[MAXITEMS];
    int num_doors;
    Door* doors[MAXDOORS];

};


typedef struct {
    gScale scale;

    // valid cords maybe ?
    // gCord validlocs[];

    int num_rooms;
    Room* rooms[MAXROOMS];
    Corridor* corridors[MAXCORRIDORS];
} Map;

void initializeMap(Map*);
Map* getMapInstance();


#endif
