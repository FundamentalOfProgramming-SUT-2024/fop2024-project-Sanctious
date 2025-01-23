#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "logger.h"
#include "map.h"
#include "item.h"
#include "config.h"
#include "structures.h"

#define RANDOM(min, max) ((rand() % ((max) - (min) + 1)) + (min)) // Inclusive
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

const int qdirs[4][2] = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

const int odirs[8][2] = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1},
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};

void initializeMap();
static Map* instance = NULL;

// Singleton design
Map* getMapInstance() {
    // First call
    if (instance == NULL) {
        instance = (Map*)malloc(sizeof(Map));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", _ERROR_, __FILE__, __LINE__);
            exit(1);
        }
        initializeMap(instance);
    }
    return instance;
}

gCord getRandomCordInRoom(Room* room){
    int _rx = RANDOM(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
    int _ry = RANDOM(room->pos.gridY, room->pos.gridY + room->scale.gridH-1);
    return (gCord) {_rx, _ry};
}

Room* getRandomRoom(Map* map){
    return map->rooms[RANDOM(0,map->num_rooms-1)];
}

gCord addDirectionToPos(gCord pos, Direction dir){
    if (dir == UP) return (gCord){pos.gridX, pos.gridY-1};
    if (dir == DOWN) return (gCord){pos.gridX, pos.gridY+1};
    if (dir == LEFT) return (gCord){pos.gridX-1, pos.gridY};
    if (dir == RIGHT) return (gCord){pos.gridX+1, pos.gridY};

}

// Factory design
Room* generateRoom(Map* map, int gx, int gy, int gw, int gh, RoomType type){
    Room* _room = (Room *) malloc(1 * sizeof(Room));
    _room->type = type;
    _room->num_doors = 0;
    _room->num_items = 0;
    _room->num_structures = 0;
    _room->visited = 0;
    _room->dfsVisited = 0;

    map->rooms[map->num_rooms] = _room;
    map->num_rooms++;

    _room->pos.gridX = gx;
    _room->pos.gridY = gy;
    _room->scale.gridW = gw;
    _room->scale.gridH = gh;

    Log("Room number %d created with gx:%d gy:%d gsx:%d gsy:%d.", _DEBUG_, map->num_rooms,
        _room->pos.gridX, _room->pos.gridY, _room->scale.gridW, _room->scale.gridH);

    return _room;
}

void generateRooms(Map* map){

    int _lowerXpos, _higherXpos, _lowerYpos, _higherYpos;

    // i should replace this with a more dynamic code
    // maybe other variable names
    // adding buffer zone to one pos is enough
    // Generate map

    for (int i = 0; i < MAPDIV; i++){
        _lowerXpos = (XCELLS-1)/MAPDIV * i + ROOM_LEFTBUFFER;
        _higherXpos = (XCELLS-1)/MAPDIV * (i+1) -MIN_ROOM_WIDTH-ROOM_RIGHTBUFFER;
        for (int j = 0; j < MAPDIV; j++){
            _lowerYpos = (YCELLS-1)/MAPDIV * j + ROOM_TOPBUFFER;
            _higherYpos = (YCELLS-1)/MAPDIV * (j+1) -MIN_ROOM_HEIGHT -ROOM_BOTTOMBUFFER;


            int _x = RANDOM(_lowerXpos, _higherXpos);
            int _y = RANDOM(_lowerYpos, _higherYpos);

            // assuming that rooms dont collide
            // it is ensured that _higherXpos - _x is greater than MIN_ROOM_WIDTH
            Room* _room =generateRoom(map, _x, _y,
                RANDOM(MIN_ROOM_WIDTH, _higherXpos - _x + MIN_ROOM_WIDTH),
                RANDOM(MIN_ROOM_HEIGHT, _higherYpos - _y + MIN_ROOM_HEIGHT),
                REGULAR);

            _room->rrp.gridX = i;
            _room->rrp.gridY = j;



        }
    }
}

// Also works with outside of map cords
Room* findRoomByRRP(int _rx, int _ry){
    Map* map = getMapInstance();
    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];
        if (room == NULL) continue;
        if (room->rrp.gridX == _rx && room->rrp.gridY == _ry) return room;
    }
    return NULL;
}

static Door* findOppositeDoor(Room* room, Direction direction){
    for (int i = 0; i < room->num_doors; i++){
        Door* door = room->doors[i];
        if (abs(door->dir-direction)==2) return door;
    }
    return NULL;
}

void generateCorridor(Door* door){
    // handle duplicates
    if (door->corridor != NULL) return;

    gCord pos1 = addDirectionToPos(door->pos, door->dir);
    gCord pos2 = addDirectionToPos(door->connectedd->pos, door->connectedd->dir);

    // maybe implement valid position for corridor so that it doesnt run
    // into rooms and stuff like that

    // x unit vector
    int xuvector = 1;
    if (pos2.gridX-pos1.gridX < 0) xuvector = -1;
    // y unit vector
    int yuvector = 1;
    if (pos2.gridY-pos1.gridY < 0) yuvector = -1;
    int xprog = 0, yprog = 0;

    // Create a new corridor
    Corridor* corridor = (Corridor *) malloc(1 * sizeof(Corridor));
    // Predetermined corridor path length
    corridor->path_length = abs(pos2.gridY-pos1.gridY) + abs(pos2.gridX-pos1.gridX) + 1 + 2;

    // for now path length is constant there is no collision with other rooms
    // Add current room pos to the list
    corridor->path[0] = door->pos;
    corridor->path[corridor->path_length-1] = door->connectedd->pos;

    corridor->path[1] = pos1;
    for (int i = 2; i <= corridor->path_length-2; i++){
        int random = rand()%2;
        if (xprog == pos2.gridX-pos1.gridX) random = 0;
        if (yprog == pos2.gridY-pos1.gridY) random = 1;

        if (random) xprog += xuvector;
        else yprog += yuvector;

        corridor->path[i].gridX = pos1.gridX + xprog;
        corridor->path[i].gridY = pos1.gridY + yprog;
    }

    // Add corridor to both doors
    door->corridor = corridor;
    door->connectedd->corridor = corridor;

    // Add corridor to map
    Map* map = getMapInstance();
    map->corridors[map->num_corridors] = corridor;
    map->num_corridors++;

    Log("Corridor generated with sp: (%d, %d) ep: (%d, %d).", _DEBUG_,
         door->pos.gridX, door->pos.gridY, door->connectedd->pos.gridX, door->connectedd->pos.gridY);


}

void generateCorridors(Map* map){

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];
        for (int j = 0; j < room->num_doors; j++){
            Door* door = room->doors[j];
            // ensured that a room in that dir exists
            Room* nroom = NULL;
            if (door->dir == UP) nroom = findRoomByRRP(room->rrp.gridX, room->rrp.gridY-1);
            if (door->dir == DOWN) nroom = findRoomByRRP(room->rrp.gridX, room->rrp.gridY+1);
            if (door->dir == LEFT) nroom = findRoomByRRP(room->rrp.gridX-1, room->rrp.gridY);
            if (door->dir == RIGHT) nroom = findRoomByRRP(room->rrp.gridX+1, room->rrp.gridY);

            //a door exists
            //no duplicate corridor is handled
            door->connectedd = findOppositeDoor(nroom, door->dir);
            generateCorridor(door);

        }
    }
}

void generateDoor(int _gx, int _gy, Room* room, Direction direction){
    Door* door = (Door *) malloc(1 * sizeof(Door));

    door->parentr = room;
    door->dir = direction;
    door->corridor = NULL;
    door->pos.gridX = _gx;
    door->pos.gridY = _gy;
    room->doors[room->num_doors] = door;
    room->num_doors++;

    Log("Door generated with pos: (%d, %d).", _DEBUG_,
         door->pos.gridX, door->pos.gridY);
}

void generateDoors(Map* map){
    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];


        int _rx1, _rx2, _ry1, _ry2, _x1, _x2, _y1, _y2;
        /*
                 ___rx1_
                |       |
                |       |
            _ry1|       |_ry2
                |       |
                |_______|
                   _rx2

        */
        // Fixed pos
        _x1 = room->pos.gridX-1;
        _x2 = room->pos.gridX + room->scale.gridW;
        _y1 = room->pos.gridY-1;
        _y2 = room->pos.gridY + room->scale.gridH;
        // Random door pos
        _rx1 = RANDOM(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
        _rx2 = RANDOM(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
        _ry1 = RANDOM(room->pos.gridY, room->pos.gridY + room->scale.gridH-1);
        _ry2 = RANDOM(room->pos.gridY, room->pos.gridY + room->scale.gridH-1);
        // Remove left door
        if (findRoomByRRP(room->rrp.gridX-1, room->rrp.gridY) == NULL) _ry1 = -1;
        // Remove right door
        if (findRoomByRRP(room->rrp.gridX+1, room->rrp.gridY) == NULL) _ry2 = -1;
        // Remove top door
        if (findRoomByRRP(room->rrp.gridX, room->rrp.gridY-1) == NULL) _rx1 = -1;
        // Remove bottom door
        if (findRoomByRRP(room->rrp.gridX, room->rrp.gridY+1) == NULL) _rx2 = -1;

        // top door
        if (_rx1 != -1){
            generateDoor(_rx1, _y1, room, UP);
        }
        // bottom door
        if (_rx2 != -1){
            generateDoor(_rx2, _y2, room, DOWN);
        }
        // left door
        if (_ry1 != -1){
            generateDoor(_x1, _ry1, room, LEFT);
        }
        // right door
        if (_ry2 != -1){
            generateDoor(_x2, _ry2, room, RIGHT);
        }

    }




//    for (int i = 0; i < )

}

void dfsMap(Room* room){
//    if (room->dfsVisited) return;
    room->dfsVisited = 1;
    // It will return NULL if it doesnt exist
    Room* neigh = findRoomByRRP(room->rrp.gridX, room->rrp.gridY-1);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(neigh);
    neigh = findRoomByRRP(room->rrp.gridX, room->rrp.gridY+1);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(neigh);
    neigh = findRoomByRRP(room->rrp.gridX-1, room->rrp.gridY);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(neigh);
    neigh = findRoomByRRP(room->rrp.gridX+1, room->rrp.gridY);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(neigh);

}

void deleteRandomRooms(Map* map){
    while (map->num_rooms != 2*MAPDIV*MAPDIV/3){
        for (int i = 0; i < map->num_rooms; i++){
            map->rooms[i]->dfsVisited = 0;
        }
        int randomNum = RANDOM(0,map->num_rooms-1);
        Room* room = map->rooms[randomNum];
        map->rooms[randomNum] = NULL;

        // Only one is NULL at each step
        if (map->rooms[0] == NULL)
            dfsMap(map->rooms[1]);
        else
            dfsMap(map->rooms[0]);

        int flag = 1;
        for (int i = 0; i < map->num_rooms; i++){
            if (map->rooms[i] == NULL) continue;
            if (!map->rooms[i]->dfsVisited){
                flag = 0;
                break;
            }
        }
        map->rooms[randomNum] = room;

        if (flag){
            Log("Deleted room: (%d, %d).", _DEBUG_, room->pos.gridX, room->pos.gridY);
            free(room);
            for (int i = randomNum; i < map->num_rooms-1; i++){
                map->rooms[i] = map->rooms[i+1];
            }
            map->num_rooms--;
        }


    }
    // Cleanup
    for (int i = 0; i < map->num_rooms; i++){
        map->rooms[i]->dfsVisited = 0;
    }
}

void checkIntegrityOfMap(Map* map){
    // Implement later
    // check check structure numbers overflowing and other stuff
}


void generateStructures(Map* map){
    for (int i = 0; i < 10; i++){
        // check item and structure overlapping each other
//        getRandomCordInRoom(getRandomRoom())
        Structure* trap = generateTrap();
        Room* room = getRandomRoom(map);
        trap->pos = getRandomCordInRoom(room);

        room->structures[room->num_structures] = trap;
        room->num_structures++;
        Log("Trap generated with pos: (%d, %d) damage: %d.", _DEBUG_,
             trap->pos.gridX, trap->pos.gridY, ((TrapExtra* )trap->StructureExtra)->damage);
    }
}

void generateItems(Map* map){
    // Implement later
}

void initializeMap(Map* map){
    srand(time(NULL));

    map->num_rooms = 0;
    map->num_corridors = 0;
    map->scale.gridW = XCELLS;
    map->scale.gridH = YCELLS;

    generateRooms(map);
    deleteRandomRooms(map);
    generateDoors(map);
    generateCorridors(map);
    generateStructures(map);
    generateItems(map);
    checkIntegrityOfMap(map);

    Log("Map initialized successfully.", _DEBUG_);

}
