#ifndef MAP_H
#define MAP_H

#include "item.h"
// its a bad practice but i have now other idea
#define MAXITEMS 64
#define MAXROOMS 16
// maybe define a coordinate system ?

typedef enum {
    TREASURE,
    NORMAL,
    TRAP
} RoomType;


typedef struct {
    int gridXPosition;
    int gridYPosition;

    int gridWidth;
    int gridHeight;

    RoomType type;

    int num_items;
    Item* items[MAXITEMS];

    // implement doors
//    Item doors[MAXITEMS];
} Room;

typedef struct {
    int gridWidth;
    int girdHeight;

    int num_rooms;
    Room* rooms[MAXROOMS];
} Map;

void initializeMap(Map*);
Map* getMapInstance();


void renderRoom(Room*);

#endif
