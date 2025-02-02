#ifndef MISCS_H
#define MISCS_H

typedef struct Item Item;

typedef enum {
    GOLD_BLACK,
    GOLD_NORMAL,
} GoldClass;

typedef struct{
    GoldClass subclass;
} GoldExtra;

Item* createGold(Item* baseItem, GoldClass subclass);
int GoldOnPickup(Item* item);

#endif
