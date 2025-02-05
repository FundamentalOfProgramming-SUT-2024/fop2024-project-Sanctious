#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "logger.h"
#include "map.h"
#include "item.h"
#include "allitems.h"
#include "config.h"
#include "structures.h"
#include "savesystem.h"
#include "utils.h"

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

static Map* floors[MAX_FLOORS];
static int curFloor = 0;
static int numFloors = 5;

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

void generateMap();
void generateLastFloor();
static Map* instance = NULL;

int getCurFloor(){
    return curFloor;
}

void setCurFloor(int floor){
    curFloor = floor;
}

int getNumFloors(){
    return numFloors;
}

void changeFloor(int floorIndex){
    if (floorIndex <= getNumFloors()-1 && 0 <= floorIndex){
        Log("Changed floor from %d to %d", _DEBUG_, getCurFloor(), floorIndex);
        curFloor = floorIndex;
    }
    else{
        Log("Floor %d doesn't exist!", _DEBUG_, floorIndex);
    }
}

Map* getFloor(int floor){
    return floors[floor];
}

void setFloor(int floor, Map* map){
    floors[floor] = map;
}
Structure* findStairs(Map* floor){
    for (int i = 0; i < floor->num_rooms; i++){
        Room* room = floor->rooms[i];
        for (int j = 0; j < room->num_structures; j++){
            Structure* structure = room->structures[j];
            if (structure->type == ST_STAIRS) return structure;
        }

    }
    return NULL;
}
// Singleton design

Room* getTreasureRoom(Map* map){
    for (int i = 0; i < map->num_rooms; i++){
        if (map->rooms[i]->type == RT_TREASURE){
            return map->rooms[i];
        }
    }
    return NULL;
}

Structure* getStairsInRoom(Room* room){
    for (int i = 0; i < room->num_structures; i++){
        if (room->structures[i]->type == ST_STAIRS) return room->structures[i];
    }

    return NULL;
}

void generateFloors(){
    curFloor = 0;
    numFloors = randomRange(4,5);
//    int p = 0;
    for (int i = 0; i < numFloors; i++){
        Map* map = (Map *) malloc(1 * sizeof(Map));
        map->id = i;
        generateMap(map);
        floors[i] = map;
    }

    // Link stairs
    Map* map = (Map *) malloc(1 * sizeof(Map));
    map->id = numFloors;
    generateLastFloor(map);
    floors[numFloors++] = map;

    for (int i = 0; i < numFloors-1; i++){
        Room* room1;
        do{
            room1 = getRandomRoom(floors[i]);
        }
        while ( getStairsInRoom(room1) != NULL);
        // Staircase in treasure room
        if (getTreasureRoom(floors[i]) != NULL){
            room1 = getTreasureRoom(floors[i]);
        }
        Room* room2 = getRandomRoom(floors[i+1]);

        Structure* stairs1 = generateBaseStructure("#", COLOR_LAVENDER, getRandomCordInRoom(room1));
        room1->structures[room1->num_structures++] = generateStairs(stairs1, i);

        Structure* stairs2 = generateBaseStructure("#", COLOR_LAVENDER, getRandomCordInRoom(room2));
        room2->structures[room2->num_structures++] = generateStairs(stairs2, i+1);

        ((StairsExtra *) stairs1->StructureExtra)->nextPos = stairs2->pos;
        ((StairsExtra *) stairs2->StructureExtra)->prevPos = stairs1->pos;

    }

    // Map validation
//    for (int i = 0; i < numFloors; i++){
//        Map* map = floors[i];
//        for (int j = 0; j < map->num_rooms; j++){
//            if (map->rooms[j]->type == RT_TREASURE){
//                p++;
//            }
//        }
//    }
//    printf("%d\n", p);
}


Map* getMapInstance() {
    // First call
    if (instance == NULL) {
        instance = (Map*)malloc(sizeof(Map));
        if (instance == NULL) {
            Log("MEM alloc failed f:%s l:%d", _ERROR_, __FILE__, __LINE__);
            exit(1);
        }
        generateFloors();
//        generateMap(instance);
//        saveUser(NULL);
    }
//    return instance;
    return NULL;
}

gCord getRandomCordInRoom(Room* room){
    int _rx = randomRange(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
    int _ry = randomRange(room->pos.gridY, room->pos.gridY + room->scale.gridH-1);
    return (gCord) {_rx, _ry};
}

Room* getRandomRoom(Map* map){
    return map->rooms[randomRange(0,map->num_rooms-1)];
}

gCord addDirectionToPos(gCord pos, Direction dir){
    if (dir == UP) return (gCord){pos.gridX, pos.gridY-1};
    if (dir == DOWN) return (gCord){pos.gridX, pos.gridY+1};
    if (dir == LEFT) return (gCord){pos.gridX-1, pos.gridY};
    if (dir == RIGHT) return (gCord){pos.gridX+1, pos.gridY};

}

void addEntityToMap(Map* map, Entity* entity){
    map->entities[map->num_entities++] = entity;
}

void removeEntityFromMap(Map* map, int entityIndex){
    for (int i = entityIndex; i < map->num_entities-1; i++){
        map->entities[i] = map->entities[i+1];
    }
    map->num_entities--;
}

void addItemToRoom(Room* room, Item* item){
    room->items[room->num_items++] = item;
}

void removeItemFromRoom(Room* room, int itemIndex){
    for (int i = itemIndex; i < room->num_items-1; i++){
        room->items[i] = room->items[i+1];
    }
    room->num_items--;
}

void addStructureToRoom(Room* room, Structure* structure){
    room->structures[room->num_structures++] = structure;

}
void removeStructureFromRoom(Room* room, int structureIndex){
    for (int i = structureIndex; i < room->num_structures-1; i++){
        room->structures[i] = room->structures[i+1];
    }
    room->num_structures--;
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

    // Room theme
    switch(type){
    case RT_TREASURE:
        _room->wallsColor = (Color) {1.0f, 0.8f, 0.0f, 1.0f};
        _room->floorsColor = (Color) {0.8f, 0.6f, 0.0f, 1.0f};
        break;
    case RT_REGULAR:
        // _room->floorsColor = (Color) {0.5, 0.5, 0.5, 1};
        _room->wallsColor = (Color) {0.7f, 0.6f, 0.4f, 1.0f};
        _room->floorsColor = (Color) {0.5f, 0.4f, 0.3f, 1.0f};
        break;
    case RT_ENCHANT:
        _room->wallsColor = (Color) {0.5f, 0.0f, 0.3f, 1.0f};
        _room->floorsColor = (Color) {0.2f, 0.2f, 0.6f, 0.5f};
        break;
    case RT_NIGHTMARE:
        _room->wallsColor = (Color) {0.4f, 0.0f, 0.0f, 1.0f};
        _room->floorsColor = (Color) {0.3f, 0.0f, 0.0f, 0.6f};
        break;
    }

    Log("Room number %d generated with \tpos: (%d, %d) \tsize: (%d, %d) \ttype: %d.", _DEBUG_, map->num_rooms,
        _room->pos.gridX, _room->pos.gridY, _room->scale.gridW, _room->scale.gridH, _room->type);

    return _room;
}

void generateRooms(Map* map, int xcells, int ycells, int division){

    int _lowerXpos, _higherXpos, _lowerYpos, _higherYpos;
    int treasure = 0;
    // i should replace this with a more dynamic code
    // maybe other variable names
    // adding buffer zone to one pos is enough
    // Generate map

    for (int i = 0; i < division; i++){
        _lowerXpos = (xcells-1)/division * i + ROOM_LEFTBUFFER;
        _higherXpos = (xcells-1)/division * (i+1) -MIN_ROOM_WIDTH-ROOM_RIGHTBUFFER;
        for (int j = 0; j < division; j++){
            _lowerYpos = (ycells-1)/division * j + ROOM_TOPBUFFER;
            _higherYpos = (ycells-1)/division * (j+1) -MIN_ROOM_HEIGHT -ROOM_BOTTOMBUFFER;


            int _x = randomRange(_lowerXpos, _higherXpos);
            int _y = randomRange(_lowerYpos, _higherYpos);

            // assuming that rooms dont collide
            // it is ensured that _higherXpos - _x is greater than MIN_ROOM_WIDTH

            // Treasure room
            RoomType rt = (RoomType) weightedRandom(1,3, (int[]) {5, 1, 1});
            if (map->id == getNumFloors()-1 && treasure == 0 && randomRange((i+1)*(j+1), division*division) == division*division){
                rt = RT_TREASURE;
                treasure = 1;
            }

            Room* _room =generateRoom(map, _x, _y,
                randomRange(MIN_ROOM_WIDTH, _higherXpos - _x + MIN_ROOM_WIDTH),
                randomRange(MIN_ROOM_HEIGHT, _higherYpos - _y + MIN_ROOM_HEIGHT),
                rt);

            _room->rrp.gridX = i;
            _room->rrp.gridY = j;
            _room->visited = 0;



        }
    }
}

// Also works with outside of map cords
Room* findRoomByRRP(Map* map, int _rx, int _ry){
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

void generateCorridor(Map* map, Door* door){
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

    for (int i = 0; i < corridor->path_length; i++){
        corridor->visited[i] = 0;
    }
    // for now path length is constant there is no collision with other rooms
    // Add current room pos to the list
    corridor->path[0] = door->pos;
    corridor->path[corridor->path_length-1] = door->connectedd->pos;

    corridor->path[1] = pos1;
    for (int i = 2; i <= corridor->path_length-2; i++){
        int randomRange = rand()%2;
        if (xprog == pos2.gridX-pos1.gridX) randomRange = 0;
        if (yprog == pos2.gridY-pos1.gridY) randomRange = 1;

        if (randomRange) xprog += xuvector;
        else yprog += yuvector;

        corridor->path[i].gridX = pos1.gridX + xprog;
        corridor->path[i].gridY = pos1.gridY + yprog;
    }

    // Add corridor to both doors
    door->corridor = corridor;
    door->connectedd->corridor = corridor;

    // Add corridor to map
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
            if (door->dir == UP) nroom = findRoomByRRP(map, room->rrp.gridX, room->rrp.gridY-1);
            if (door->dir == DOWN) nroom = findRoomByRRP(map, room->rrp.gridX, room->rrp.gridY+1);
            if (door->dir == LEFT) nroom = findRoomByRRP(map, room->rrp.gridX-1, room->rrp.gridY);
            if (door->dir == RIGHT) nroom = findRoomByRRP(map, room->rrp.gridX+1, room->rrp.gridY);

            //a door exists
            //no duplicate corridor is handled
            door->connectedd = findOppositeDoor(nroom, door->dir);
            generateCorridor(map, door);

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
        _rx1 = randomRange(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
        _rx2 = randomRange(room->pos.gridX, room->pos.gridX + room->scale.gridW-1);
        _ry1 = randomRange(room->pos.gridY, room->pos.gridY + room->scale.gridH-1);
        _ry2 = randomRange(room->pos.gridY, room->pos.gridY + room->scale.gridH-1);
        // Remove left door
        if (findRoomByRRP(map, room->rrp.gridX-1, room->rrp.gridY) == NULL) _ry1 = -1;
        // Remove right door
        if (findRoomByRRP(map, room->rrp.gridX+1, room->rrp.gridY) == NULL) _ry2 = -1;
        // Remove top door
        if (findRoomByRRP(map, room->rrp.gridX, room->rrp.gridY-1) == NULL) _rx1 = -1;
        // Remove bottom door
        if (findRoomByRRP(map, room->rrp.gridX, room->rrp.gridY+1) == NULL) _rx2 = -1;

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

void dfsMap(Map* map, Room* room){
//    if (room->dfsVisited) return;
    room->dfsVisited = 1;
    // It will return NULL if it doesnt exist
    Room* neigh = findRoomByRRP(map, room->rrp.gridX, room->rrp.gridY-1);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(map, neigh);
    neigh = findRoomByRRP(map, room->rrp.gridX, room->rrp.gridY+1);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(map, neigh);
    neigh = findRoomByRRP(map, room->rrp.gridX-1, room->rrp.gridY);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(map, neigh);
    neigh = findRoomByRRP(map, room->rrp.gridX+1, room->rrp.gridY);
    if (neigh != NULL && neigh->dfsVisited == 0) dfsMap(map, neigh);

}

void deleteRoom(Map* map, Room* room, int roomIndex){
    Log("Deleted room: (%d, %d).", _DEBUG_, room->pos.gridX, room->pos.gridY);
    free(room);
    for (int i = roomIndex; i < map->num_rooms-1; i++){
        map->rooms[i] = map->rooms[i+1];
    }
    map->num_rooms--;
}

void deleteRandomRooms(Map* map){
    while (map->num_rooms != 2*MAPDIV*MAPDIV/3){
        for (int i = 0; i < map->num_rooms; i++){
            map->rooms[i]->dfsVisited = 0;
        }
        int randomNum = randomRange(0,map->num_rooms-1);
        Room* room = map->rooms[randomNum];

        // Don't remove the treasure room
        if (room->type == RT_TREASURE) continue;

        map->rooms[randomNum] = NULL;

        // Only one is NULL at each step
        if (map->rooms[0] == NULL)
            dfsMap(map, map->rooms[1]);
        else
            dfsMap(map, map->rooms[0]);

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
            deleteRoom(map, room, randomNum);
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

// Generate treasure-room props
void gps_TreasureRoom(Room* room){
    Item* item;

    for (int i = 0; i < 5; i++ ){


        if (randomRange(1, 2) == 1){
            item = createBaseItem("Gold", getRandomCordInRoom(room), "G", (Color) {1.0, 0.84, 0.0, 1.0}, 10);
            addItemToRoom(room, createGold(item, GOLD_NORMAL));
        }

        if (randomRange(1, 5) == 1){
            item = createBaseItem("Black Gold", getRandomCordInRoom(room), "G", (Color) {0.85, 0.65, 0.13, 1.0}, 10);
            addItemToRoom(room, createGold(item, GOLD_BLACK));
        }

        Structure* structure = generateBaseStructure("\u010c", (Color) {1.0f, 0.0f, 0.0f, 1.0f}, getRandomCordInRoom(room));
        addStructureToRoom(room, generateTrap(structure, randomRange(3,7)));
    }
}
// Generate nightmare-room props
void gps_NightmareRoom(Map* map, Room* room){
    Entity* entity;
    for (int i = 0; i < 2; i++){
    if (randomRange(1, 5) == 1){
        entity = createEntity("Snake", 20, getRandomCordInRoom(room), "\u0105", (Color) {0.0f, 0.6f, 0.0f, 1.0f});
        addEntityToMap(map, createSnake(entity, 5));
    }

    if (randomRange(1, 2) == 1){
        entity = createEntity("Demon", 5, getRandomCordInRoom(room), "\u0106", (Color) {0.6f, 0.0f, 0.2f, 1.0f});
        addEntityToMap(map, createDemon(entity, 1, 5));
    }
    if (randomRange(1, 5) == 1){
        entity = createEntity("Dragon", 10, getRandomCordInRoom(room), "\u0107", (Color) {1.0f, 0.0f, 0.0f, 1.0f});
        addEntityToMap(map, createDragon(entity, 2, 5));
    }

    if (randomRange(1, 7) == 1){
        entity = createEntity("Undead", 30, getRandomCordInRoom(room), "\u0108", (Color) {0.5f, 0.5f, 0.5f, 1.0f});
        addEntityToMap(map, createUndead(entity, 10, 10));
    }

    if (randomRange(1, 5) == 1){
        entity = createEntity("Giant", 15, getRandomCordInRoom(room), "\u0109", (Color) {0.8f, 0.4f, 0.1f, 1.0f});
        addEntityToMap(map, createGiant(entity, 3, 10));
    }
    }
}
// Generate regular-room props
void gps_RegularRoom(Map* map, Room* room){
    Structure* structure;
    Item* item;
    Entity* entity;

    for (int i = 0; i < 3; i++){
        if (randomRange(1,3) == 1){
            structure = generateBaseStructure("\u010c", (Color) {1.0f, 0.0f, 0.0f, 1.0f}, getRandomCordInRoom(room));
            addStructureToRoom(room, generateTrap(structure, randomRange(3,7)));
        }
    }
    if (randomRange(1, 5) == 1){
        item = createBaseItem("Mace", getRandomCordInRoom(room), "\u010d", (Color) {0.6f, 0.5f, 0.3f, 1.0f}, -1);
        addItemToRoom(room, createMeleeWeapon(item, MELEEWEAPON_MACE, 5));
    }

    if (randomRange(1, 5) == 1){
        item = createBaseItem("Sword", getRandomCordInRoom(room), "\u0104", (Color) {0.8f, 0.8f, 0.8f, 1.0f}, 2);
        addItemToRoom(room, createMeleeWeapon(item, MELEEWEAPON_SWORD, 10));
    }
        // {0.5f, 0.0f, 0.5f, 1.0f} magical staff
        // {0.7f, 0.4f, 0.1f, 1.0f} arrows
    if (randomRange(1, 5) == 1){
        item = createBaseItem("Normal Food", getRandomCordInRoom(room), "\u0103", COLOR_BROWN, 2);
        addItemToRoom(room, createFood(item, FOOD_NORMAL, 10));
    }
    for (int i = 0; i < 3; i++){
        if (randomRange(1, 2) == 1){
            item = createBaseItem("Gold", getRandomCordInRoom(room), "G", (Color) {1.0, 0.84, 0.0, 1.0}, 10);
            addItemToRoom(room, createGold(item, GOLD_NORMAL));
        }

        if (randomRange(1, 5) == 1){
            item = createBaseItem("Black Gold", getRandomCordInRoom(room), "G", (Color) {0.85, 0.65, 0.13, 1.0}, 10);
            addItemToRoom(room, createGold(item, GOLD_BLACK));
        }
    }

    if (randomRange(1, 5) == 1){
        entity = createEntity("Snake", 20, getRandomCordInRoom(room), "\u0105", (Color) {0.0f, 0.6f, 0.0f, 1.0f});
        addEntityToMap(map, createSnake(entity, 5));
    }

    if (randomRange(1, 2) == 1){
        entity = createEntity("Demon", 5, getRandomCordInRoom(room), "\u0106", (Color) {0.6f, 0.0f, 0.2f, 1.0f});
        addEntityToMap(map, createDemon(entity, 1, 5));
    }

    if (randomRange(1, 2) == 1){
        entity = createEntity("Dragon", 10, getRandomCordInRoom(room), "\u0107", (Color) {1.0f, 0.0f, 0.0f, 1.0f});
        addEntityToMap(map, createDragon(entity, 2, 5));
    }

    if (randomRange(1, 5) == 1){
        entity = createEntity("Giant", 15, getRandomCordInRoom(room), "\u0109", (Color) {0.8f, 0.4f, 0.1f, 1.0f});
        addEntityToMap(map, createGiant(entity, 3, 10));
    }
}
// Generate enchant-room props
void gps_EnchantRoom(Room* room){
    Item* item;

    for (int i = 0; i < 3; i++ ){
        if (randomRange(1, 3) == 1){
            item = createBaseItem("Heal Potion", getRandomCordInRoom(room), "\u010B", COLOR_CRIMSON, 1);
            addItemToRoom(room, createPotion(item, POTION_HEAL, 2, 10));
        }

        if (randomRange(1, 3) == 1){
            item = createBaseItem("Speed Potion", getRandomCordInRoom(room), "\u010B", COLOR_CYAN, 1);
            addItemToRoom(room, createPotion(item, POTION_SPEED, 2, 10));
        }

        if (randomRange(1, 3) == 1){
            item = createBaseItem("Strength Potion", getRandomCordInRoom(room), "\u010B", COLOR_BROWN, 1);
            addItemToRoom(room, createPotion(item, POTION_STRENGTH, 2, 10));
        }

        if (randomRange(1, 2) == 1){
            item = createBaseItem("Gold", getRandomCordInRoom(room), "G", (Color) {1.0, 0.84, 0.0, 1.0}, 10);
            addItemToRoom(room, createGold(item, GOLD_NORMAL));
        }

        if (randomRange(1, 3) == 1){
            item = createBaseItem("Normal Food", getRandomCordInRoom(room), "\u0103", COLOR_BROWN, 4);
            addItemToRoom(room, createFood(item, FOOD_NORMAL, 10));
        }
    }
    item = createBaseItem("Magical Food", getRandomCordInRoom(room), "\u0103", COLOR_CYAN, 4);
    addItemToRoom(room, createFood(item, FOOD_MAGICAL, 10));

    item = createBaseItem("Legendary Food", getRandomCordInRoom(room), "\u0103", COLOR_ORANGE, 4);
    addItemToRoom(room, createFood(item, FOOD_LEGENDARY, 10));

    item = createBaseItem("Rotten Food", getRandomCordInRoom(room), "\u0103", COLOR_ELECTRIC_BLUE, 4);
    addItemToRoom(room, createFood(item, FOOD_ROTTEN, 10));
}

void configureRooms(Map* map){
    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];
        // check item and structure overlapping each other
//        Structure* structure = generateBaseStructure("\u010c", (Color) {1.0f, 0.0f, 0.0f, 1.0f}, getRandomCordInRoom(room));
//        addStructureToRoom(room, generateTrap(structure, 5));
        switch(room->type){
        case RT_NIGHTMARE:
            gps_NightmareRoom(map, room);
            break;
        case RT_TREASURE:
            gps_TreasureRoom(room);
            break;
        case RT_REGULAR:
            gps_RegularRoom(map, room);
            break;
        case RT_ENCHANT:
            gps_EnchantRoom(room);
            break;
        }
    }
}

void generateItems(Map* map){
    return;
    for (int i = 0; i < 3; i++){
        Room* room = getRandomRoom(map);

        char temp[100];
        sprintf(temp, "Mace%d", i);
        Item* item = createBaseItem(temp, getRandomCordInRoom(room), "\u010d", (Color) {0.6f, 0.5f, 0.3f, 1.0f}, -1);
        addItemToRoom(room, createMeleeWeapon(item, MELEEWEAPON_MACE, 5));

        sprintf(temp, "Arrow%d", i);
        item = createBaseItem(temp, getRandomCordInRoom(room), "\u0104", (Color) {0.8f, 0.8f, 0.8f, 1.0f}, 2);
        addItemToRoom(room, createMeleeWeapon(item, MELEEWEAPON_SWORD, 10));
        // {0.5f, 0.0f, 0.5f, 1.0f} magical staff
        // {0.7f, 0.4f, 0.1f, 1.0f} arrows

        sprintf(temp, "Food %d", i);
        item = createBaseItem(temp, getRandomCordInRoom(room), "\u0103", COLOR_BROWN, 2);
        addItemToRoom(room, createFood(item, FOOD_NORMAL, 10));

        sprintf(temp, "Gold %d", i);
        item = createBaseItem(temp, getRandomCordInRoom(room), "G", COLOR_GOLD, 10);
        addItemToRoom(room, createGold(item, GOLD_NORMAL));

        sprintf(temp, "Gold %d", i);
        item = createBaseItem(temp, getRandomCordInRoom(room), "G", COLOR_EMERALD, 10);
        addItemToRoom(room, createGold(item, GOLD_BLACK));
    }
}

void generateEntities(Map* map){
    for (int i = 0; i < 5; i++){
        Room* room = getRandomRoom(map);
        Entity* entity;
        if (randomRange(1, 5) == 1){
            entity = createEntity("Snake", 20, getRandomCordInRoom(room), "\u0105", (Color) {0.0f, 0.6f, 0.0f, 1.0f});
            addEntityToMap(map, createSnake(entity, 5));
        }
        if (randomRange(1, 2) == 1){
            entity = createEntity("Demon", 5, getRandomCordInRoom(room), "\u0106", (Color) {0.6f, 0.0f, 0.2f, 1.0f});
            addEntityToMap(map, createDemon(entity, 1, 5));
        }
        if (randomRange(1, 5) == 1){
            entity = createEntity("Dragon", 10, getRandomCordInRoom(room), "\u0107", (Color) {1.0f, 0.0f, 0.0f, 1.0f});
            addEntityToMap(map, createDragon(entity, 2, 5));
        }

        if (randomRange(1, 7) == 1){
            entity = createEntity("Undead", 30, getRandomCordInRoom(room), "\u0108", (Color) {0.5f, 0.5f, 0.5f, 1.0f});
            addEntityToMap(map, createUndead(entity, 10, 10));
        }

        if (randomRange(1, 5) == 1){
            entity = createEntity("Giant", 15, getRandomCordInRoom(room), "\u0109", (Color) {0.8f, 0.4f, 0.1f, 1.0f});
            addEntityToMap(map, createGiant(entity, 3, 10));
        }
    }
}

void configureLastFloor(Map* map){
    Structure* structure;

    for (int i = 0; i < 10; i++){
        Room* room = getRandomRoom(map);
        structure = generateBaseStructure("\u010c", (Color) {1.0f, 0.0f, 0.0f, 1.0f}, getRandomCordInRoom(room));
        addStructureToRoom(room, generateTrap(structure, randomRange(3,7)));
    }

}

void generateMap(Map* map){

    map->num_rooms = 0;
    map->num_entities = 0;
    map->num_corridors = 0;
    map->scale.gridW = XCELLS;
    map->scale.gridH = YCELLS;

    generateRooms(map, XCELLS, YCELLS, MAPDIV);
    deleteRandomRooms(map);
    generateDoors(map);
    generateCorridors(map);

    configureRooms(map);
    generateItems(map);

    checkIntegrityOfMap(map);

    Log("Floor-%d initialized successfully.", _DEBUG_, map->id);

}

void generateLastFloor(Map* map){
    map->num_rooms = 0;
    map->num_entities = 0;
    map->num_corridors = 0;
    map->scale.gridW = XCELLS;
    map->scale.gridH = YCELLS;

    generateRooms(map, XCELLS, YCELLS, 1);
    generateEntities(map);
    configureLastFloor(map);

    Log("Floor-%d initialized successfully.", _DEBUG_, map->id);
}
