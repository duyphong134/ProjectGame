#include "Music.h"
#include <SDL.h>


Mix_Music* Music::loadMusic(const char* path)
{
        gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
}


void Music::playMusic(Mix_Music *gMusic)
{
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
}


// sound

Mix_Chunk* Music::loadSound(const char* path)
{
        Mix_Chunk *gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
               "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return gChunk;
}

void Music::playSound(Mix_Chunk* gChunk)
{
    if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
    }

}

void Music::changeMusicState()
{
     if( Mix_PlayingMusic() == 0 )
        {
            Mix_PlayMusic( gMusic, -1 );
        }
     else{
        if( Mix_PausedMusic() == 1 ){
            Mix_ResumeMusic();
        }
        else{
            Mix_PauseMusic();
            }
    }
}
