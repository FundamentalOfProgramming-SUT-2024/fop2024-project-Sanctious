#ifndef MAP_H
#define MAP_H

#include "item.h"
#include "config.h"

extern const int qdirs[4][2]; // 4 neighs quad
extern const int odirs[8][2]; // 8 neighs octa

struct Room;
typedef struct Room Room;

struct Door;
typedef struct Door Door;

typedef enum {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
} Direction;

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
    Direction dir; // direction relative to the room
    gCord pos; // pos in parent room

    Corridor* corridor;
    Room* parentr; // parent room
    Door* connectedd; // connected door
} ;

struct Room{
    gCord pos;
    gScale scale;
    gCord rrp; // relative room position in rooms grid

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
    int num_corridors;
    Corridor* corridors[MAXCORRIDORS];
} Map;

void initializeMap(Map*);
Map* getMapInstance();
Room* findRoomByRRP(int, int);


#endif
