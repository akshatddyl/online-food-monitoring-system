#ifndef ORDER_H
#define ORDER_H

typedef struct {
    int orderId;
    char customerName[50];
    int foodId;
    int quantity;
    char status[20];
} Order;

extern Order orders[];
extern int orderCount;

void loadOrders();
void saveOrder(Order o);
void placeOrder(char customerName[]);
void updateOrderStatus();
void showOrders();

#endif
