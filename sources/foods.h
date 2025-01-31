#ifndef FOODS_H
#define FOODS_H

typedef struct Item Item;

typedef enum {
    FOOD_NORMAL,
    FOOD_MAGICAL,
} FoodClass;

typedef struct{
    FoodClass subclass;

    int hunger;
} FoodExtra;

Item* createFood(Item* baseItem, FoodClass subclass, int hunger);
int FoodOnConsume(Item* item);

#endif
