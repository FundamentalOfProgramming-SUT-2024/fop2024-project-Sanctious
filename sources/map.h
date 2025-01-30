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
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3
} Direction;

typedef enum {
    RT_TREASURE,
    RT_REGULAR,
    RT_ENCHANT,
    RT_NIGHTMARE
} RoomType;

typedef enum {
    DIFFICULTY_LOSER,
    DIFFICULTY_MID,
    DIFFICULTY_NERD,
    DIFFICULTY_GAMER
} Difficulty;

typedef enum {
    MS_NORMAL,
    MS_LARGE,
    MS_GAMER,
    MS_SIGMABOI
} MapSize;

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
    Color wallsColor;
    Color floorsColor;
    // Visited by player or not
    char visited;

    // Used in map generation
    char dfsVisited;

    RoomType type;

    int num_structures;
    Structure* structures[MAXSTRUCTURES];
    int num_items;
    Item* items[MAXITEMS];
    int num_doors;
    Door* doors[MAXDOORS];

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

Room* findRoomByRRP(Map*, int, int);
gCord getRandomCordInRoom(Room* room);
gCord addDirectionToPos(gCord pos, Direction dir);
Room* getRandomRoom(Map* map);

#endif
