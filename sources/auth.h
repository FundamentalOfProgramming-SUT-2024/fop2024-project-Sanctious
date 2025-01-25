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
    int exp;
    int num_games;
} Stats;


typedef struct{
    Credentials creds;
    Stats stats;
    char saves[MAX_SAVES_PU][MAX_STR_SIZE];
} User;

User* getCurrentUser();
void setCurrentUser(User* user);
int isValidPassword(char* password);
int isValidEmail(char* email);
int userExists(char* name);
User* createUser(char* name, char* password, char* email);
User* loadUser(char* name, char* password);

#endif
