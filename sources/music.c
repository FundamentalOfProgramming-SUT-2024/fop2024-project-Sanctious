#include "music.h"
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

Mix_Music *music;

void initMusic(char* path) {
    struct sigaction action;

    sigaction(SIGINT, NULL, &action);
    SDL_Init(SDL_INIT_AUDIO);
    sigaction(SIGINT, &action, NULL);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, AUDIO_F32SYS, 2, 1024);
    playMusic(path);
}

void stopMusic(){
    Mix_HaltMusic();
}

void playMusic(char* path){
    music = Mix_LoadMUS(path);
    Mix_PlayMusic(music, -1);
}

void shutdownMusic() {
    Mix_FreeMusic(music);
    Mix_Quit();
    SDL_Quit();
}