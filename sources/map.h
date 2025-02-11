#ifndef MAP_H
#define MAP_H

#include "item.h"
#include "config.h"
#include "structures.h"
#include "entities.h"
#include "gc.h"

extern const int qdirs[4][2]; // 4 neighs quad
extern const int odirs[8][2]; // 8 neighs octa

struct Room;
typedef struct Room Room;

struct Door;
typedef struct Door Door;

typedef enum {
    UP      = 0,
    LEFT    = 1,
    DOWN    = 2,
    RIGHT   = 3,
    NONE    = 4,
} Direction;

typedef enum {
    RT_TREASURE     = 0,
    RT_REGULAR      = 1,
    RT_ENCHANT      = 2,
    RT_NIGHTMARE    = 3
} RoomType;

typedef struct {
    int gridW;
    int gridH;

} gScale;

typedef struct {
    int path_length;
    gCord path[MAX_CORRIDOR_LENGTH];
    int visited[MAX_CORRIDOR_LENGTH];
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
    Color wallsColor;
    Color floorsColor;

    // Used in map generation
    char dfsVisited;

    RoomType type;

    int num_structures;
    Structure* structures[MAXSTRUCTURES];
    int num_items;
    Item* items[MAXITEMS];
    int num_doors;
    Door* doors[MAXDOORS];

    // Visited by player or not
    int visited;
};

typedef struct {
    int id;
    gScale scale;

    // valid cords maybe ?
    // gCord validlocs[];
    int num_entities;
    Entity* entities[MAXENTITIES];
    int num_rooms;
    Room* rooms[MAXROOMS];
    int num_corridors;
    Corridor* corridors[MAXCORRIDORS];
} Map;

void generateMap(Map*);
Map* getMapInstance();



Map* getFloor(int floor);
void changeFloor(int a);
int getCurFloor();
void generateFloors();
void setFloor(int floor, Map* map);
int getNumFloors();
void setCurFloor(int floor);
Structure* getStairsInRoom(Room* room);
int isInsideRoom(gCord pos);

void addEntityToMap(Map* map, Entity* entity);
void removeEntityFromMap(Map* map, int entityIndex);
int findEntityIndex(Map* map, Entity* entity);
void addItemToRoom(Room* room, Item* item);
void removeItemFromRoom(Room* room, int itemIndex);
void addStructureToRoom(Room* room, Structure* structure);
void removeStructureFromRoom(Room* room, int structureIndex);


Room* findRoomByRRP(Map*, int, int);
gCord getRandomCordInRoom(Room* room);
gCord addDirectionToPos(gCord pos, Direction dir);
Room* getRandomRoom(Map* map);

#endif
