#include <strings.h>
#include "auth.h"
#include "main.h"

User* getUserByName(char* name){
    for (int i = 0; i < 10; i++){
        if (strcmp(NULL, name) == 0)
            return NULL;
    }
    return NULL;
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

void loadUsers(){


}



#include "stdlib.h"
#include "stdio.h"
void saveUser(User* user){
    user = (User *) malloc(sizeof(User));

    user->stats.num_games = 255;
    FILE *file = fopen("test.dat", "wb");
    fwrite(user, sizeof(User), 1, file);
    fclose(file);

    file = fopen("test.dat", "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fread(user, sizeof(User), 1, file);
    printf("**%d**", user->stats.num_games);
    fclose(file);
}




