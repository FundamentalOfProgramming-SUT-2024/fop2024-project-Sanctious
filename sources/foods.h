#ifndef FOODS_H
#define FOODS_H

typedef struct Item Item;

typedef enum {
    FOOD_NORMAL,
    FOOD_MAGICAL,
    FOOD_LEGENDARY,
    FOOD_ROTTEN,
} FoodClass;

typedef struct{
    FoodClass subclass;

    int hungerPoints;
} FoodExtra;

Item* createFood(Item* baseItem, FoodClass subclass, int hungerPoints);
int FoodOnConsume(Item* item);
int FoodOnPickup(Item* item);

#endif
