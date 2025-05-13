#include <stdio.h>
#include <string.h>
#include "inventory.h"
#include "utils.h"

FoodItem items[100];
int itemCount = 0;

void loadInventory() {
    FILE *fp = fopen("inventory.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%d %s %f %d", &items[itemCount].id, items[itemCount].name, &items[itemCount].price, &items[itemCount].quantity) != EOF) {
        itemCount++;
    }
    fclose(fp);
}

void saveInventory() {
    FILE *fp = fopen("inventory.txt", "w");
    for (int i = 0; i < itemCount; i++) {
        fprintf(fp, "%d %s %.2f %d\n", items[i].id, items[i].name, items[i].price, items[i].quantity);
    }
    fclose(fp);
}

void addItem() {
    FoodItem f;
    printf(CYAN "Adding New Food Item:\n" RESET);
    printf("Enter ID: "); scanf("%d", &f.id);
    printf("Enter Name: "); scanf("%s", f.name);
    printf("Enter Price: "); scanf("%f", &f.price);
    printf("Enter Quantity: "); scanf("%d", &f.quantity);

    items[itemCount++] = f;
    saveInventory();
    printf(GREEN "Item added successfully.\n" RESET);
}

void updateStock() {
    int id, qty;
    printf("Enter Item ID to update: ");
    scanf("%d", &id);

    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id) {
            printf("Enter new quantity: ");
            scanf("%d", &qty);
            items[i].quantity = qty;
            saveInventory();
            printf(GREEN "Stock updated.\n" RESET);
            return;
        }
    }
    printf(RED "Item not found.\n" RESET);
}

void showInventory() {
    printf(YELLOW "\n--- INVENTORY ---\n" RESET);
    for (int i = 0; i < itemCount; i++) {
        printf("%d\t%s\t₹%.2f\tQty: %d\n", items[i].id, items[i].name, items[i].price, items[i].quantity);
        if (items[i].quantity < 5)
            printf(RED "⚠️ Low stock alert for %s!\n" RESET, items[i].name);
    }
}
