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
    REGULAR,
    ENCHANT,
    NIGHTMARE
} RoomType;

typedef enum {
    LOSER_DIFFICULTY,
    MID_DIFFICULTY,
    NERD_DIFFICULTY,
    GAMER_DIFFICULTY
} Difficulty;

typedef enum {
    NORMAL_SIZE,
    LARGE_SIZE,
    GAMER_SIZE,
    SIGMABOI_SIZE
} MapSize;

typedef struct {
    int gridX;
    int gridY;

} gCord;

typedef struct {
    int gridW;
    int gridH;

} gScale;
////
// Structures
typedef enum {
    WINDOW,
    OBSTACLE,
    TRAP
} StructureType;


typedef struct{
    int damage;
} TrapExtra;

typedef struct{
    gCord pos;
    char sprite;
    void* StructureExtra;
    StructureType type;

} Structure;

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
