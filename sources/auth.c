#include <stdlib.h>
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
    #include <dirent.h>
#endif

void createDirectory(char* path) {
#ifdef _WIN32
    _mkdir(path);
#else
    mkdir(path, 0700);
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

    user->stats = (Stats) {0, 0, 0, 10, 0};
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

    Log("User created successfully username: \"%s\".", _DEBUG_, name);
    return user;

}

User* updateUser(User* user){
    char temp[MAX_STR_SIZE];
    sprintf(temp, "saves/%s/userdata.dat", getCurrentUser()->creds.name);

    FILE* file = fopen(temp, "wb");
    fwrite(getCurrentUser(), sizeof(User), 1, file);
    fclose(file);

    Log("Updated user: \"%s\".", _DEBUG_, user->creds.name);
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
        Log("User successfully loaded username: \"%s\".", _DEBUG_, name);
        return user;
    }
    else {
        Log("Loading user failed username: \"%s\".", _DEBUG_, name);
        return NULL;
    }

}

int loadAllUsers(User* users[MAX_NUM_USERS]){
    int count = 0;  // Count of directories found
    const char* path = "./saves";
    char temp[MAX_STRING_INPUT+1];

    #ifdef _WIN32
    WIN32_FIND_DATA findData;
    char searchPath[512];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", path);

    HANDLE hFind = FindFirstFile(searchPath, &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        perror("Error opening directory");
        return -1;
    }

    do {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (strcmp(findData.cFileName, ".") != 0 && strcmp(findData.cFileName, "..") != 0) {
                strncpy(temp, findData.cFileName, MAX_STRING_INPUT);
                temp[MAX_STRING_INPUT] = '\0'; // Ensure null termination
                users[count] = loadUser(temp, NULL);
                count++;
//                if (count >= MAX_NUM_USERS) break;
            }
        }
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);

    #else  // Linux/macOS
    struct dirent *entry;
    struct stat statbuf;
    DIR *dir = opendir(path);
    if (!dir) {
        perror("Error opening directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (stat(fullPath, &statbuf) == 0 && S_ISDIR(statbuf.st_mode)) {
            if (entry->d_name[0] != '.') {  // Ignore "." and ".."
                strncpy(temp, entry->d_name, MAX_STRING_INPUT);
                temp[MAX_STRING_INPUT] = '\0';  // Ensure null termination
                users[count] = loadUser(temp, NULL);
                count++;
//                if (count >= MAX_NUM_USERS) break;
            }
        }
    }
    closedir(dir);
    #endif

    return count;
}
