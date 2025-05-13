#ifndef USER_H
#define USER_H

int login();
void registerUser();
void loadUsers();
void setLoggedInUser(char *username, char *role);
extern char loggedInUser[50];
extern char loggedInRole[20];

#endif
