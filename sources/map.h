#ifndef MAP_H
#define MAP_H

#include "item.h"
// its a bad practice but i have now other idea
#define MAXDOORS 16
#define MAXITEMS 64

// maybe define a coordinate system ?

typedef struct {
    int gridWidth;
    int girdHeight;
} Map;

void initializeMap();
Map* getMapInstance();

typedef struct {
    int gridXPosition;
    int gridYPosition;

    int gridWidth;
    int gridHeight;

    int num_items;
    Item items[MAXITEMS];
//    Item doors[MAXITEMS];
} Room;

void renderRoom(Room*);

#endif
