#ifndef INVENTORY_H
#define INVENTORY_H

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} FoodItem;

extern FoodItem items[];
extern int itemCount;

void loadInventory();
void saveInventory();
void addItem();
void updateStock();
void showInventory();

#endif
