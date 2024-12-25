#ifndef PLAYER_H
#define PLAYER_H


typedef struct {

    int armor;
    int gold;
} Player;


void initializePlayer();
Player* getPlayerInstance();

#endif
