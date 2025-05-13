#include <stdio.h>
#include <string.h>
#include "user.h"
#include "utils.h"

typedef struct {
    char username[50];
    char password[50];
    char role[20];
} User;

User users[100];
int userCount = 0;

char loggedInUser[50];
char loggedInRole[20];

void setLoggedInUser(char *username, char *role) {
    strcpy(loggedInUser, username);
    strcpy(loggedInRole, role);
}

void loadUsers() {
    FILE *fp = fopen("users.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%s %s %s", users[userCount].username, users[userCount].password, users[userCount].role) != EOF) {
        userCount++;
    }
    fclose(fp);
}

void registerUser() {
    User u;
    printf(GREEN "Registering New User\n" RESET);
    printf("Choose role (admin/staff/customer): ");
    scanf("%s", u.role);
    printf("Choose username: ");
    scanf("%s", u.username);
    printf("Choose password: ");
    scanf("%s", u.password);

    FILE *fp = fopen("users.txt", "a");
    fprintf(fp, "%s %s %s\n", u.username, u.password, u.role);
    fclose(fp);

    printf(GREEN "User registered successfully!\n" RESET);
}

int login() {
    char username[50], password[50];
    printf(BLUE "Login\n" RESET);
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            setLoggedInUser(username, users[i].role);
            printf(GREEN "Login successful as %s!\n" RESET, users[i].role);
            return 1;
        }
    }
    printf(RED "Invalid credentials.\n" RESET);
    return 0;
}
