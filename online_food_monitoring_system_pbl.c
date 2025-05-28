#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// ANSI Color Codes for colorful interface
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

// Structure for User Authentication
typedef struct {
    char username[50];
    char password[50];
    char role[20]; // admin, staff, customer
} User;

// Structure for Food Items
typedef struct {
    int food_id;
    char name[100];
    float price;
    int quantity;
    char category[50];
} FoodItem;

// Structure for Orders
typedef struct {
    int order_id;
    char customer_name[50];
    int food_id;
    char food_name[100];
    int quantity;
    float total_price;
    char status[30]; // Preparing, Out for Delivery, Delivered
} Order;

// Structure for Feedback
typedef struct {
    int feedback_id;
    char customer_name[50];
    int order_id;
    int rating;
    char comment[200];
} Feedback;

// Global variables
User current_user;
int next_order_id = 1;
int next_feedback_id = 1;

// Function prototypes
void display_header();
void delay_screen();
int authenticate_user();
int register_user();
int check_username_exists(char *username);
void save_user_to_file(User user);
void admin_menu();
void staff_menu();
void customer_menu();
void view_orders();
void update_order_status();
void add_food_item();
void view_inventory();
void check_low_stock();
void browse_menu();
void place_order();
void submit_feedback();
void view_feedback();
int get_next_food_id();
int check_food_id_exists(int food_id);

int main() {
    int choice;
    
    while(1) {
        system("clear"); // Clear screen
        display_header();
        
        printf(CYAN "🔐 Authentication Menu\n" RESET);
        printf(WHITE "1. 📝 Register\n");
        printf("2. 🔑 Login\n");
        printf("3. ❌ Exit\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                if(register_user()) {
                    printf(GREEN "✅ Registration successful! Please login now.\n" RESET);
                    delay_screen();
                }
                break;
            case 2:
                if(authenticate_user()) {
                    // Based on role, show different menus
                    if(strcmp(current_user.role, "admin") == 0) {
                        admin_menu();
                    } else if(strcmp(current_user.role, "staff") == 0) {
                        staff_menu();
                    } else if(strcmp(current_user.role, "customer") == 0) {
                        customer_menu();
                    }
                }
                break;
            case 3:
                printf(GREEN "👋 Thank you for using our system!\n" RESET);
                exit(0);
            default:
                printf(RED "❌ Invalid choice! Please try again.\n" RESET);
                delay_screen();
        }
    }
    
    return 0;
}

// Display system header with creator name
void display_header() {
    printf(BOLD MAGENTA "🍽️  ========================================\n");
    printf("   ONLINE FOOD MONITORING SYSTEM\n");
    printf("       Made by: Akshat Dhondiyal\n");
    printf("   ========================================\n" RESET);
    printf("\n");
}

// Add delay for better user experience
void delay_screen() {
    printf(YELLOW "Press Enter to continue..." RESET);
    int c;
    // Clear input buffer until newline or EOF
    while ((c = getchar()) != '\n' && c != EOF) { }
    // Wait for actual Enter press
    getchar();
    sleep(1);
}

// Check if username already exists in user.txt
int check_username_exists(char *username) {
    // Open user.txt in read mode
    FILE *file = fopen("user.txt", "r");
    if(file == NULL) return 0;  // File doesn't exist = no users yet
    
    User temp_user;
    // Read user records: username password role
    while(fscanf(file, "%s %s %s", temp_user.username, temp_user.password, temp_user.role) == 3) {
        // Compare input username with stored username
        if(strcmp(temp_user.username, username) == 0) {
            fclose(file);
            return 1;  // Username found
        }
    }
    fclose(file);
    return 0;  // Username not found
}

// Save new user to file
void save_user_to_file(User user) {
    FILE *file = fopen("user.txt", "a");
    if(file == NULL) {
        printf(RED "❌ Error opening file!\n" RESET);
        return;
    }
    fprintf(file, "%s %s %s\n", user.username, user.password, user.role);
    fclose(file);
}

// User registration function
int register_user() {
    User new_user;
    int role_choice;
    
    printf(CYAN "\n📝 User Registration\n" RESET);
    printf("Enter username: ");
    scanf("%s", new_user.username);
    
    // Check if username already exists
    if(check_username_exists(new_user.username)) {
        printf(RED "❌ Username already exists! Please choose a different username or login.\n" RESET);
        delay_screen();
        return 0;
    }
    
    printf("Enter password: ");
    scanf("%s", new_user.password);
    
    printf(YELLOW "Select your role:\n");
    printf("1. 👨‍💼 Admin\n");
    printf("2. 👩‍🍳 Staff\n");
    printf("3. 🙋‍♂️ Customer\n" RESET);
    printf("Enter choice (1-3): ");
    scanf("%d", &role_choice);
    
    switch(role_choice) {
        case 1: strcpy(new_user.role, "admin"); break;
        case 2: strcpy(new_user.role, "staff"); break;
        case 3: strcpy(new_user.role, "customer"); break;
        default:
            printf(RED "❌ Invalid role selection!\n" RESET);
            delay_screen();
            return 0;
    }
    
    save_user_to_file(new_user);
    return 1;
}

// User authentication function
int authenticate_user() {
    char username[50], password[50];
    FILE *file = fopen("user.txt", "r");
    
    if(file == NULL) {
        printf(RED "❌ No users registered yet!\n" RESET);
        delay_screen();
        return 0;
    }
    // Get login credentials
    printf(CYAN "\n🔑 User Login\n" RESET);
    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);
    // Check credentials against stored users
    User temp_user;
    while(fscanf(file, "%s %s %s", temp_user.username, temp_user.password, temp_user.role) == 3) {
        // If credentials match
        if(strcmp(temp_user.username, username) == 0 && strcmp(temp_user.password, password) == 0) {
            // Copy user data to global current_user
            strcpy(current_user.username, temp_user.username);
            strcpy(current_user.password, temp_user.password);
            strcpy(current_user.role, temp_user.role);
            fclose(file);
            printf(GREEN "✅ Login successful! Welcome %s (%s)\n" RESET, current_user.username, current_user.role);
            delay_screen();
            return 1;
        }
    }
    
    fclose(file);
    printf(RED "❌ Invalid username or password!\n" RESET);
    delay_screen();
    return 0;
}

// Admin menu with all access
void admin_menu() {
    int choice;
    
    while(1) {
        system("clear");
        printf(BOLD RED "👨‍💼 ADMIN DASHBOARD\n" RESET);
        printf(GREEN "Welcome %s!\n\n" RESET, current_user.username);
        
        printf(WHITE "1. 📋 View All Orders\n");
        printf("2. 🔄 Update Order Status\n");
        printf("3. ➕ Add Food Item\n");
        printf("4. 📦 View Inventory\n");
        printf("5. ⚠️  Check Low Stock Alerts\n");
        printf("6. 💬 View Customer Feedback\n");
        printf("7. 🚪 Logout\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: view_orders(); break;
            case 2: update_order_status(); break;
            case 3: add_food_item(); break;
            case 4: view_inventory(); break;
            case 5: check_low_stock(); break;
            case 6: view_feedback(); break;
            case 7: 
                printf(GREEN "👋 Logging out...\n" RESET);
                delay_screen();
                return;
            default:
                printf(RED "❌ Invalid choice!\n" RESET);
                delay_screen();
        }
    }
}

// Staff menu with limited access
void staff_menu() {
    int choice;
    
    while(1) {
        system("clear");
        printf(BOLD BLUE "👩‍🍳 STAFF DASHBOARD\n" RESET);
        printf(GREEN "Welcome %s!\n\n" RESET, current_user.username);
        
        printf(WHITE "1. 📋 View Orders\n");
        printf("2. 🔄 Update Order Status\n");
        printf("3. ➕ Add Food Item\n");
        printf("4. 📦 View Inventory\n");
        printf("5. ⚠️  Check Low Stock Alerts\n");
        printf("6. 🚪 Logout\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: view_orders(); break;
            case 2: update_order_status(); break;
            case 3: add_food_item(); break;
            case 4: view_inventory(); break;
            case 5: check_low_stock(); break;
            case 6: 
                printf(GREEN "👋 Logging out...\n" RESET);
                delay_screen();
                return;
            default:
                printf(RED "❌ Invalid choice!\n" RESET);
                delay_screen();
        }
    }
}

// Customer menu with customer-specific options
void customer_menu() {
    int choice;
    
    while(1) {
        system("clear");
        printf(BOLD GREEN "🙋‍♂️ CUSTOMER DASHBOARD\n" RESET);
        printf(GREEN "Welcome %s!\n\n" RESET, current_user.username);
        
        printf(WHITE "1. 🍽️  Browse Menu\n");
        printf("2. 🛒 Place Order\n");
        printf("3. 💬 Submit Feedback\n");
        printf("4. 🚪 Logout\n" RESET);
        printf(YELLOW "Enter your choice: " RESET);
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: browse_menu(); break;
            case 2: place_order(); break;
            case 3: submit_feedback(); break;
            case 4: 
                printf(GREEN "👋 Logging out...\n" RESET);
                delay_screen();
                return;
            default:
                printf(RED "❌ Invalid choice!\n" RESET);
                delay_screen();
        }
    }
}

// View all orders function
void view_orders() {
    FILE *file = fopen("orders.txt", "r");
    if(file == NULL) {
        printf(RED "❌ No orders found!\n" RESET);
        delay_screen();
        return;
    }
    
    printf(CYAN "\n📋 ALL ORDERS\n" RESET);
    printf("==================================================\n");
    
    Order order;
    while(fscanf(file, "%d %s %d %s %d %f %s", &order.order_id, order.customer_name, 
                 &order.food_id, order.food_name, &order.quantity, &order.total_price, order.status) == 7) {
        printf("Order ID: %d | Customer: %s | Food: %s\n", order.order_id, order.customer_name, order.food_name);
        printf("Quantity: %d | Total: Rs.%.2f | Status: %s\n", order.quantity, order.total_price, order.status);
        printf("--------------------------------------------------\n");
    }
    
    fclose(file);
    delay_screen();
}

// Update order status function
void update_order_status() {
    int order_id, found = 0;
    char new_status[30];
    
    printf(CYAN "\n🔄 UPDATE ORDER STATUS\n" RESET);
    printf("Enter Order ID: ");
    scanf("%d", &order_id);
    
    printf("Enter new status (Preparing/Out_for_Delivery/Delivered): ");
    scanf("%s", new_status);
    
    FILE *file = fopen("orders.txt", "r");
    FILE *temp = fopen("temp_orders.txt", "w");
    
    if(file == NULL) {
        printf(RED "❌ No orders found!\n" RESET);
        delay_screen();
        return;
    }
    
    Order order;
    while(fscanf(file, "%d %s %d %s %d %f %s", &order.order_id, order.customer_name, 
                 &order.food_id, order.food_name, &order.quantity, &order.total_price, order.status) == 7) {
        if(order.order_id == order_id) {
            strcpy(order.status, new_status);
            found = 1;
        }
        fprintf(temp, "%d %s %d %s %d %.2f %s\n", order.order_id, order.customer_name, 
                order.food_id, order.food_name, order.quantity, order.total_price, order.status);
    }
    
    fclose(file);
    fclose(temp);
    
    remove("orders.txt");
    rename("temp_orders.txt", "orders.txt");
    
    if(found) {
        printf(GREEN "✅ Order status updated successfully!\n" RESET);
    } else {
        printf(RED "❌ Order ID not found!\n" RESET);
    }
    delay_screen();
}

// Get next available food ID
int get_next_food_id() {
    // Open inventory file in read mode
    FILE *file = fopen("inventory.txt", "r");
    if(file == NULL) return 1;  // If no file exists, start with ID no.1
    
    int max_id = 0;
    FoodItem item;
    // Read each food item from file
    while(fscanf(file, "%d %s %f %d %s", &item.food_id, item.name, 
                 &item.price, &item.quantity, item.category) == 5) {
        // Update max_id if current item has higher ID
        if(item.food_id > max_id) {
            max_id = item.food_id;
        }
    }
    fclose(file);
    return max_id + 1;  // Return next available ID
}

// Check if food ID already exists
int check_food_id_exists(int food_id) {
    FILE *file = fopen("inventory.txt", "r");
    if(file == NULL) return 0;
    
    FoodItem item;
    while(fscanf(file, "%d %s %f %d %s", &item.food_id, item.name, &item.price, &item.quantity, item.category) == 5) {
        if(item.food_id == food_id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

// Add new food item to inventory
void add_food_item() {
    FoodItem new_item;
    
    printf(CYAN "\n➕ ADD NEW FOOD ITEM\n" RESET);
    
    printf("Enter Food ID: ");
    scanf("%d", &new_item.food_id);
    
    // Check if food ID already exists
    if(check_food_id_exists(new_item.food_id)) {
        printf(RED "❌ Food ID already exists! Please use a different ID.\n" RESET);
        printf(YELLOW "Suggested next ID: %d\n" RESET, get_next_food_id());
        delay_screen();
        return;
    }
    //Asking for Details about new food item going to get added
    printf("Enter Food Name: ");
    scanf("%s", new_item.name);
    printf("Enter Price (Rs.): ");
    scanf("%f", &new_item.price);
    printf("Enter Quantity: ");
    scanf("%d", &new_item.quantity);
    printf("Enter Category: ");
    scanf("%s", new_item.category);
    
    FILE *file = fopen("inventory.txt", "a");
    if(file == NULL) {
        printf(RED "❌ Error opening file!\n" RESET);
        delay_screen();
        return;
    }
    
    fprintf(file, "%d %s %.2f %d %s\n", new_item.food_id, new_item.name, new_item.price, new_item.quantity, new_item.category);
    fclose(file);
    
    printf(GREEN "✅ Food item added successfully!\n" RESET);
    delay_screen();
}

// View inventory (for admin/staff with low stock alerts)
void view_inventory() {
    FILE *file = fopen("inventory.txt", "r");
    if(file == NULL) {
        printf(RED "❌ No inventory found!\n" RESET);
        delay_screen();
        return;
    }
    
    printf(CYAN "\n📦 INVENTORY MANAGEMENT\n" RESET);
    printf("================================================================\n");
    
    FoodItem item;
    while(fscanf(file, "%d %s %f %d %s", &item.food_id, item.name, &item.price, &item.quantity, item.category) == 5) {
        printf("ID: %d | Name: %s | Price: Rs.%.2f\n", item.food_id, item.name, item.price);
        printf("Quantity: %d | Category: %s", item.quantity, item.category);
        
        // Show low stock warning for admin/staff
        if(item.quantity < 10) {
            printf(RED " ⚠️  LOW STOCK!" RESET);
        }
        printf("\n----------------------------------------------------------------\n");
    }
    
    fclose(file);
    delay_screen();
}

// Check low stock alerts (admin/staff only)
void check_low_stock() {
    FILE *file = fopen("inventory.txt", "r");
    if(file == NULL) {
        printf(RED "❌ No inventory found!\n" RESET);
        delay_screen();
        return;
    }
    
    printf(RED "\n⚠️  LOW STOCK ALERTS\n" RESET);
    printf("================================\n");
    
    int low_stock_count = 0;
    FoodItem item;
    while(fscanf(file, "%d %s %f %d %s", &item.food_id, item.name, &item.price, &item.quantity, item.category) == 5) {
        if(item.quantity < 10) {
            printf("🚨 %s (ID: %d) - Only %d left!\n", item.name, item.food_id, item.quantity);
            low_stock_count++;
        }
    }
    
    if(low_stock_count == 0) {
        printf(GREEN "✅ All items are well stocked!\n" RESET);
    }
    
    fclose(file);
    delay_screen();
}

// Browse menu (customer view - no stock info)
void browse_menu() {
    FILE *file = fopen("inventory.txt", "r");
    if(file == NULL) {
        printf(RED "❌ Menu not available!\n" RESET);
        delay_screen();
        return;
    }
    
    printf(CYAN "\n🍽️  RESTAURANT MENU\n" RESET);
    printf("================================\n");
    
    FoodItem item;
    while(fscanf(file, "%d %s %f %d %s", &item.food_id, item.name, &item.price, &item.quantity, item.category) == 5) {
        if(item.quantity > 0) { // Only show available items
            printf("🍽️  %s (ID: %d)\n", item.name, item.food_id);
            printf("   Price: Rs.%.2f | Category: %s\n", item.price, item.category);
            printf("--------------------------------\n");
        }
    }
    
    fclose(file);
    delay_screen();
}

// Place order function
void place_order() {
    int food_id, quantity;
    float price = 0;
    char food_name[100];
    int found = 0;
    
    printf(CYAN "\n🛒 PLACE ORDER\n" RESET);
    
    // First show menu
    browse_menu();
    
    printf("Enter Food ID to order: ");
    scanf("%d", &food_id);
    printf("Enter quantity: ");
    scanf("%d", &quantity);
    
    // Check if food item exists and has enough stock
    FILE *file = fopen("inventory.txt", "r");
    FILE *temp = fopen("temp_inventory.txt", "w");
    
    if(file == NULL) {
        printf(RED "❌ Menu not available!\n" RESET);
        delay_screen();
        return;
    }
    
    FoodItem item;
    while(fscanf(file, "%d %s %f %d %s", &item.food_id, item.name, &item.price, &item.quantity, item.category) == 5) {
        if(item.food_id == food_id) {
            if(item.quantity >= quantity) {
                strcpy(food_name, item.name);
                price = item.price;
                item.quantity -= quantity;
                found = 1;
            } else {
                printf(RED "❌ Insufficient stock! Available: %d\n" RESET, item.quantity);
                fclose(file);
                fclose(temp);
                remove("temp_inventory.txt");
                delay_screen();
                return;
            }
        }
        fprintf(temp, "%d %s %.2f %d %s\n", item.food_id, item.name, item.price, item.quantity, item.category);
    }
    
    fclose(file);
    fclose(temp);
    
    if(!found) {
        printf(RED "❌ Food item not found!\n" RESET);
        remove("temp_inventory.txt");
        delay_screen();
        return;
    }
    
    // Update inventory
    remove("inventory.txt");
    rename("temp_inventory.txt", "inventory.txt");
    
    // Save order
    FILE *order_file = fopen("orders.txt", "a");
    if(order_file != NULL) {
        fprintf(order_file, "%d %s %d %s %d %.2f Preparing\n", next_order_id, current_user.username, 
                food_id, food_name, quantity, price * quantity);
        fclose(order_file);
        
        printf(GREEN "✅ Order placed successfully!\n" RESET);
        printf("Order ID: %d | Total: Rs.%.2f\n", next_order_id, price * quantity);
        next_order_id++;
    } else {
        printf(RED "❌ Error placing order!\n" RESET);
    }
    
    delay_screen();
}

// Submit feedback function
void submit_feedback() {
    int order_id, rating;
    char comment[200];
    
    printf(CYAN "\n💬 SUBMIT FEEDBACK\n" RESET);
    printf("Enter Order ID: ");
    scanf("%d", &order_id);
    printf("Enter Rating (1-5): ");
    scanf("%d", &rating);
    printf("Enter Comment: ");
    getchar(); // Clear buffer
    fgets(comment, sizeof(comment), stdin);
    
    FILE *file = fopen("feedback.txt", "a");
    if(file == NULL) {
        printf(RED "❌ Error saving feedback!\n" RESET);
        delay_screen();
        return;
    }
    
    fprintf(file, "%d %s %d %d %s", next_feedback_id, current_user.username, order_id, rating, comment);
    fclose(file);
    
    printf(GREEN "✅ Thank you for your feedback!\n" RESET);
    next_feedback_id++;
    delay_screen();
}

// View feedback (admin only)
void view_feedback() {
    FILE *file = fopen("feedback.txt", "r");
    if(file == NULL) {
        printf(RED "❌ No feedback found!\n" RESET);
        delay_screen();
        return;
    }
    
    printf(CYAN "\n💬 CUSTOMER FEEDBACK\n" RESET);
    printf("=====================================\n");
    
    Feedback feedback;
    while(fscanf(file, "%d %s %d %d", &feedback.feedback_id, feedback.customer_name, 
                 &feedback.order_id, &feedback.rating) == 4) {
        fgets(feedback.comment, sizeof(feedback.comment), file);
        printf("Feedback ID: %d | Customer: %s\n", feedback.feedback_id, feedback.customer_name);
        printf("Order ID: %d | Rating: %d/5\n", feedback.order_id, feedback.rating);
        printf("Comment: %s", feedback.comment);
        printf("-------------------------------------\n");
    } 
    
    fclose(file);
    delay_screen();
}
