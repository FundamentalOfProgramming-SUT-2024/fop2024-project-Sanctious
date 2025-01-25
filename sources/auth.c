#include <strings.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "auth.h"
#include "logger.h"
#include "main.h"
#include "map.h"

#ifdef _WIN32
    #include <direct.h>
    #include <windows.h>
#else
    #include <sys/stat.h>
#endif

void createDirectory(char* path) {
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(directoryName, 0700);
#endif
}

static User* currentUser = NULL;

User* getCurrentUser(){
    return currentUser;
}
void setCurrentUser(User* user){
    currentUser = user;
}

int isValidEmail(char* email){
    char temp[MAX_STR_SIZE];
    strcpy(temp, email);
    char* token;
    int sections = 0;
    token = strtok(temp, ".@");
    while (token != NULL) {
        sections++;
        token = strtok(NULL, ".@");
    }
    return sections == 3;
}


int isValidPassword(char* password){
    if (strlen(password) < PASSWORD_MINLENGTH) return 0;

    int flag = 0;
    for (int i = 0; i < strlen(password); i++){
        if ('0' <= password[i] && password[i] <= '9') flag |= 1 << 0;
        if ('a' <= password[i] && password[i] <= 'z') flag |= 1 << 1;
        if ('A' <= password[i] && password[i] <= 'Z') flag |= 1 << 2;
    }

    return (flag == 7);

}

int userExists(char* name){
    char temp[100];
    sprintf(temp, "saves/%s/userdata.dat", name);

    FILE* file = fopen(temp, "rb");

    return (file != NULL);
}

int comparePasswords(User* user, char* password){
    // Implement string hashing
    return !strcmp(user->creds.password, password);
}

User* createUser(char* name, char* password, char* email){
    // Doesn't check for already existing users
    User* user = (User *) malloc(1 * sizeof(User));
    strcpy(user->creds.name, name);
    strcpy(user->creds.password, password);
    strcpy(user->creds.email, email);

    user->stats = (Stats) {0, 0, 0, 0, 0};
    char temp[MAX_STR_SIZE];
    sprintf(temp, "saves/%s", name);
    createDirectory(temp);

    sprintf(temp, "saves/%s/userdata.dat", name);

    FILE* file = fopen(temp, "wb");
    if (file == NULL) {
        Log("Error opening file %s %d", _DEBUG_, __FILE__, __LINE__);
        return NULL;
    }

    fwrite(user, sizeof(User), 1, file);
    fclose(file);

    Log("Register successful username: \"%s\".", _DEBUG_, name);
    return user;

}

User* loadUser(char* name, char* password){
    User* user = (User *) malloc(1 * sizeof(User));

    char temp[MAX_STR_SIZE];
    sprintf(temp, "saves/%s/userdata.dat", name);

    FILE* file = fopen(temp, "rb");
    if (file == NULL) {
        Log("User with name \"%s\" doesn't exist.", _DEBUG_, name);
        return NULL;
    }

    fread(user, sizeof(User), 1, file);
    fclose(file);

    // Load user without password
    if (password == NULL || comparePasswords(user, password)){
        Log("Login successful username: \"%s\".", _DEBUG_, name);
        return user;
    }
    else {
        Log("Login failed username: \"%s\".", _DEBUG_, name);
        return NULL;
    }

}
