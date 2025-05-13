#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"
#include "inventory.h"
#include "order.h"
#include "feedback.h"
#include "utils.h"

int main() {
    loadUsers();
    loadInventory();
    loadOrders();
    loadFeedbacks();

    int choice;
    printf(BLUE "Welcome to the Online Food Monitoring System\n" RESET);
    printf("1. Register\n2. Login\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) registerUser();
    if (!login()) return 0;

    while (1) {
        printf(YELLOW "\n--- MAIN MENU (%s) ---\n" RESET, loggedInRole);

        if (strcmp(loggedInRole, "admin") == 0 || strcmp(loggedInRole, "staff") == 0) {
            printf("1. Add Food Item\n");
            printf("2. Update Stock\n");
            printf("3. View Inventory\n");
            printf("4. View Orders\n");
            printf("5. Update Order Status\n");
            printf("6. View Feedback\n");
            printf("7. Exit\n");
            printf("Enter choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: addItem(); break;
                case 2: updateStock(); break;
                case 3: showInventory(); break;
                case 4: showOrders(); break;
                case 5: updateOrderStatus(); break;
                case 6: viewFeedback(); break;
                case 7: printf(GREEN "Goodbye!\n" RESET); exit(0);
                default: printf(RED "Invalid choice.\n" RESET);
            }

        } else if (strcmp(loggedInRole, "customer") == 0) {
            printf("1. View Menu\n");
            printf("2. Place Order\n");
            printf("3. View Orders\n");
            printf("4. Give Feedback\n");
            printf("5. Exit\n");
            printf("Enter choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1: showInventory(); break;
                case 2: placeOrder(loggedInUser); break;
                case 3: showOrders(); break;
                case 4: giveFeedback(loggedInUser); break;
                case 5: printf(GREEN "Goodbye!\n" RESET); exit(0);
                default: printf(RED "Invalid choice.\n" RESET);
            }
        }
    }

    return 0;
}
