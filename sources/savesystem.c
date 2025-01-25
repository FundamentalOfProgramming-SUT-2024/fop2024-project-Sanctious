#include <strings.h>
#include <string.h>
#include <stdint.h>
#include "stdlib.h"
#include "stdio.h"
#include "savesystem.h"
#include "logger.h"
#include "player.h"
#include "main.h"
#include "map.h"


static SaveInfo* currentSave = NULL;

SaveInfo* getCurrentSave(){
    return currentSave;
}
void setCurrentSave(SaveInfo* saveinfo){
    currentSave = saveinfo;
}

#define HASHMAP_SIZE 128

typedef struct KeyValue {
    void *key;
    void *value;
    struct KeyValue *next;
} KeyValue;

typedef struct {
    KeyValue *buckets[HASHMAP_SIZE];
    unsigned int (*hash_function)(void *);
    int (*key_comparator)(void *, void *);
} HashMap;

unsigned int default_hash_function(void *key) {
    return ((uintptr_t)key) % HASHMAP_SIZE;
}

int default_key_comparator(void *key1, void *key2) {
    return key1 == key2;
}

HashMap *hashmap_init(unsigned int (*hash_function)(void *), int (*key_comparator)(void *, void *)) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (!map) {
        perror("Failed to allocate hashmap");
        exit(EXIT_FAILURE);
    }
    memset(map->buckets, 0, sizeof(map->buckets));
    map->hash_function = hash_function ? hash_function : default_hash_function;
    map->key_comparator = key_comparator ? key_comparator : default_key_comparator;
    return map;
}

void hashmap_add(HashMap *map, void *key, void *value) {
    unsigned int hash = map->hash_function(key);
    KeyValue *new_pair = (KeyValue *)malloc(sizeof(KeyValue));
    if (!new_pair) {
        perror("Failed to allocate key-value pair");
        exit(EXIT_FAILURE);
    }
    new_pair->key = key;
    new_pair->value = value;
    new_pair->next = map->buckets[hash];
    map->buckets[hash] = new_pair;
}

void *hashmap_get(HashMap *map, void *key) {
    unsigned int hash = map->hash_function(key);
    KeyValue *current = map->buckets[hash];
    while (current) {
        if (map->key_comparator(current->key, key)) {
            return current->value;
        }
        current = current->next;
    }
    return NULL; // Key not found
}

void hashmap_free(HashMap *map) {
    for (int i = 0; i < HASHMAP_SIZE; i++) {
        KeyValue *current = map->buckets[i];
        while (current) {
            KeyValue *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(map);
}

//int main() {
//
//
//    int key1 = 1, value1 = 100;
//    char *key2 = "hello";
//    char *value2 = "world";
//
//
//    int *retrieved_value1 = (int *)hashmap_get(map, &key1);
//    char *retrieved_value2 = (char *)hashmap_get(map, key2);
//
//    printf("Retrieved value 1: %d\n", retrieved_value1 ? *retrieved_value1 : -1);
//    printf("Retrieved value 2: %s\n", retrieved_value2 ? retrieved_value2 : "not found");
//
//    hashmap_free(map);
//    return 0;
//}



void saveGame(){
    char temp[MAX_STR_SIZE];
    sprintf(temp, "saves/%s/%s.dat", getCurrentUser()->creds.name, getCurrentSave()->savename);
    FILE* file = fopen(temp, "wb");
//    fwrite(&user, sizeof(User *), 1, file);
//    fwrite(user, sizeof(User), 1, file);
    fwrite(getCurrentSave(), sizeof(SaveInfo), 1, file);

//    fwrite(&getPlayerInstance(), sizeof(Player *), 1, file);
    fwrite(getPlayerInstance(), sizeof(Player), 1, file);

    int curFloor = getCurFloor();
    fwrite(&curFloor, sizeof(int), 1, file);

    int numFloors = getNumFloors();
    printf("Numfloors : %d*B\n", numFloors);
    printf("Golds : %d*B\n", getCurrentSave()->gold);
    fwrite(&numFloors, sizeof(int), 1, file);

    for (int i = 0; i < numFloors; i++){

    Map* map = getFloor(i);



//    fwrite(&map, sizeof(Map *), 1, file);
    fwrite(map, sizeof(Map), 1, file);

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* corridor = map->corridors[i];

        fwrite(&corridor, sizeof(Corridor *), 1, file);
        fwrite(corridor, sizeof(Corridor), 1, file);

    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        fwrite(&room, sizeof(Room *), 1, file);
        fwrite(room, sizeof(Room), 1, file);

    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_doors; j++){
            Door* door = room->doors[j];

            fwrite(&door, sizeof(Door *), 1, file);
            fwrite(door, sizeof(Door), 1, file);
        }

    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_structures; j++){
            Structure* structure = room->structures[j];

            fwrite(&structure, sizeof(Structure *), 1, file);
            fwrite(structure, sizeof(Structure), 1, file);
        }



    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_structures; j++){
            Structure* structure = room->structures[j];
//            WINDOW
//            OBSTACLE
//            TRAP
            switch(structure->type){
            case TRAP:
                // what if we dont put it there
                fwrite(&(structure->StructureExtra), sizeof(TrapExtra *), 1, file);
                fwrite((TrapExtra *) structure->StructureExtra, sizeof(TrapExtra), 1, file);
                break;
            case OBSTACLE:


                break;
            case WINDOW:


                break;
            }
        }



    }
    }

//    updateUserData();

    fclose(file);
}
void loadGame(char* name){
    HashMap *hashmap = hashmap_init(NULL, NULL);
    //    Save{
//        saveinfo -> use the name here to save the file and the user
//        player
//        numfloors
//        floors
//        update userdata file ( change it and save it )
//    }
    char temp[MAX_STR_SIZE];
    sprintf(temp, "saves/%s/%s.dat", getCurrentUser()->creds.name, name);
    FILE* file = fopen(temp, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    SaveInfo* saveinfo = (SaveInfo *) malloc(1 * sizeof(SaveInfo));
    fread(saveinfo, sizeof(SaveInfo), 1, file);
    setCurrentSave(saveinfo);

    Player* player = (Player *) malloc(1 * sizeof(Player));
    fread(player, sizeof(Player), 1, file);
    setPlayerInstance(player);

    int curFloor;
    fread(&curFloor, sizeof(int), 1, file);
    setCurFloor(curFloor);

    int numFloors;
    fread(&numFloors, sizeof(int), 1, file);
    printf("Numfloors : %d\n", numFloors);
for (int i = 0; i < numFloors; i ++){

    Map* map = (Map *) malloc(1 * sizeof(Map));
    void* temp;

//    fread(&temp, sizeof(Map *), 1, file);
    fread(map, sizeof(Map), 1, file);

    for (int i = 0; i < map->num_corridors; i++){
        Corridor* corridor = (Corridor *) malloc(1 * sizeof(Corridor));

        fread(&temp, sizeof(Corridor *), 1, file);
        hashmap_add(hashmap, temp, corridor);
//        printf("%p %p %p\n", temp, map->corridors[i], hashmap_get(hashmap, temp));
        fread(corridor, sizeof(Corridor), 1, file);

    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = (Room *) malloc(1 * sizeof(Room));

        fread(&temp, sizeof(Room *), 1, file);
        hashmap_add(hashmap, temp, room);
        fread(room, sizeof(Room), 1, file);

    }

    for (int i = 0; i < map->num_rooms; i++){
        map->rooms[i] = (Room *) hashmap_get(hashmap, map->rooms[i]);
    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_doors; j++){
            Door* door = (Door *) malloc(1 * sizeof(Door));

            fread(&temp, sizeof(Door *), 1, file);
            hashmap_add(hashmap, temp, door);
            fread(door, sizeof(Door), 1, file);
        }

    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_structures; j++){
            Structure* structure = (Structure *) malloc(1 * sizeof(Structure));

            fread(&temp, sizeof(Structure *), 1, file);
            hashmap_add(hashmap, temp, structure);
            fread(structure, sizeof(Structure), 1, file);
        }



    }
    // Link room and doors
    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_structures; j++){
            room->structures[j] = (Structure *) hashmap_get(hashmap, room->structures[j]);
        }
        for (int j = 0; j < room->num_doors; j++){
            room->doors[j] = (Door *) hashmap_get(hashmap, room->doors[j]);
            room->doors[j]->connectedd = (Door *) hashmap_get(hashmap, room->doors[j]->connectedd);
            room->doors[j]->parentr = (Room *) hashmap_get(hashmap, room->doors[j]->parentr);
            room->doors[j]->corridor = (Corridor *) hashmap_get(hashmap, room->doors[j]->corridor);
        }
//        room->items

    }

    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];

        for (int j = 0; j < room->num_structures; j++){
            Structure* structure = room->structures[j];
//            WINDOW
//            OBSTACLE
//            TRAP
            if (structure->type == TRAP){
                // what if we dont put it there
                TrapExtra * extra = (TrapExtra *) malloc(1 * sizeof(TrapExtra));
                fread(&temp, sizeof(TrapExtra *), 1, file);
                hashmap_add(hashmap, temp, extra);
                fread(extra, sizeof(TrapExtra), 1, file);
            }
            else if (structure->type == OBSTACLE){


            }
            else if (structure->type == WINDOW){

            }
        }



    }

    // Link structures extra
    for (int i = 0; i < map->num_rooms; i++){
        Room* room = map->rooms[i];
        for (int j = 0; j < room->num_structures; j++){
            room->structures[j]->StructureExtra = (void *) hashmap_get(hashmap, room->structures[j]->StructureExtra);
        }

    }

    // Link map corridors
    for (int i = 0; i < map->num_corridors; i++){
        map->corridors[i] = (Corridor *) hashmap_get(hashmap, map->corridors[i]);

    }
    setFloor(i, map);
//    void* test;
//    fread(&test, sizeof(User *), 1, file);
//    fread(user, sizeof(User), 1, file);
//    printf("**%d %p**", map->num_rooms );
    }
    fclose(file);

//    return map;
}

void createSave(SaveInfo* saveinfo){
    generateFloors();
    getPlayerInstance();
    saveinfo->gold = 10;
    saveinfo->playtime = 0;

    setCurrentSave(saveinfo);
    saveGame();
}
//    load{
//        saveinfo
//        player
//        floor
//    }
//    Save{
//        saveinfo -> use the name here to save the file and the user
//        player
//        numfloors
//        floors
//        update userdata file ( change it and save it )
//    }
//    then load them back ( using load function )
//}
//
//Userdata{
//    Userdata
//        Creds
//        Stats
//        savenames[]
//
//}
//
//Save{
//    saveinfo
//    player
//    floor
//}

