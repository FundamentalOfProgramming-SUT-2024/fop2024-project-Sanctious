#ifndef MAP_H
#define MAP_H

#include "item.h"
#include "config.h"

struct Room;
typedef struct Room Room;

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
    gCord path[MAX_CORRIDOR_LENGTH];
} Corridor;

typedef struct {
    Corridor corridor;
    Room* first;
    Room* second;
} Door;

struct Room{
    gCord pos;
    gScale scale;

    RoomType type;

    int num_items;
    Item* items[MAXITEMS];
    Door* doors[MAXITEMS];

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
