#ifndef MAP_H
#define MAP_H


typedef struct {
    int data;
} Map;

void initializeMap();
Map* getMapInstance();

#endif
