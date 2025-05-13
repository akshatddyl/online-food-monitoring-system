#include <stdio.h>
#include <string.h>
#include "order.h"
#include "inventory.h"
#include "utils.h"

Order orders[100];
int orderCount = 0;

void loadOrders() {
    FILE *fp = fopen("orders.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%d %s %d %d %s", &orders[orderCount].orderId, orders[orderCount].customerName,
                  &orders[orderCount].foodId, &orders[orderCount].quantity, orders[orderCount].status) != EOF) {
        orderCount++;
    }
    fclose(fp);
}

void saveOrder(Order o) {
    FILE *fp = fopen("orders.txt", "a");
    fprintf(fp, "%d %s %d %d %s\n", o.orderId, o.customerName, o.foodId, o.quantity, o.status);
    fclose(fp);
}

void placeOrder(char customerName[]) {
    Order o;
    o.orderId = orderCount + 1;
    strcpy(o.customerName, customerName);
    printf("Enter Food ID: "); scanf("%d", &o.foodId);
    printf("Enter Quantity: "); scanf("%d", &o.quantity);

    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == o.foodId && items[i].quantity >= o.quantity) {
            items[i].quantity -= o.quantity;
            strcpy(o.status, "Preparing");
            orders[orderCount++] = o;
            saveInventory();
            saveOrder(o);
            printf(GREEN "Order placed successfully!\n" RESET);
            return;
        }
    }
    printf(RED "Item not available or insufficient quantity.\n" RESET);
}

void updateOrderStatus() {
    int id;
    char status[20];
    printf("Enter Order ID: "); scanf("%d", &id);
    for (int i = 0; i < orderCount; i++) {
        if (orders[i].orderId == id) {
            printf("Enter new status: ");
            scanf("%s", status);
            strcpy(orders[i].status, status);

            FILE *fp = fopen("orders.txt", "w");
            for (int j = 0; j < orderCount; j++) {
                fprintf(fp, "%d %s %d %d %s\n", orders[j].orderId, orders[j].customerName,
                        orders[j].foodId, orders[j].quantity, orders[j].status);
            }
            fclose(fp);
            printf(GREEN "Order status updated.\n" RESET);
            return;
        }
    }
    printf(RED "Order not found.\n" RESET);
}

void showOrders() {
    printf(BLUE "\n--- Orders ---\n" RESET);
    for (int i = 0; i < orderCount; i++) {
        printf("ID:%d | Customer:%s | FoodID:%d | Qty:%d | Status:%s\n",
               orders[i].orderId, orders[i].customerName, orders[i].foodId,
               orders[i].quantity, orders[i].status);
    }
}
