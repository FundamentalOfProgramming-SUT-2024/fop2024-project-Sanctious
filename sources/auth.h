#ifndef AUTH_H
#define AUTH_H

#include "config.h"
#include "map.h"

typedef struct{
    char password[PASSWORD_MAXLENGTH];
    char name[MAX_STRING_INPUT];
    char email[MAX_STRING_INPUT];

} Credentials;

typedef struct{
    long long playTime; // In seconds
    long long sumScores;
    long long sumGold;
    int test;
    int exp;
    int num_games;
} Stats;


typedef struct{
    Credentials creds;
    Stats stats;
} User;


User* getUserByName(char* name);
int isValidPassword(char* password);
int isValidEmail(char* email);
Map* saveUser(User* user);
void loadUsers();

#endif
