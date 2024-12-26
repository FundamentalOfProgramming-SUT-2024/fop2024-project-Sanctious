#ifndef MAP_H
#define MAP_H

#include "item.h"
#include "config.h"
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
    int gridHeight;

    int num_rooms;
    Room* rooms[MAXROOMS];
} Map;

void initializeMap(Map*);
Map* getMapInstance();


#endif
