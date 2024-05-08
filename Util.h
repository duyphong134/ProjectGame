#ifndef UTIL_H

#include "InGame.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>



void InGame::logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}

void InGame::init(SDL_Window* game_window, SDL_Renderer* game_renderer)
{

    window = game_window;
    render = game_renderer;

                SDL_Surface* loadSurf = IMG_Load("Assets\\images\\background.png");
                background = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                loadSurf = IMG_Load("Assets\\images\\blocks.png");
                blocks = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                loadSurf = IMG_Load("Assets\\images\\score_frame.png");
                scoreframe = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                loadSurf = IMG_Load("Assets\\images\\ghostBlocks.png");
                SDL_SetColorKey(loadSurf, SDL_TRUE, SDL_MapRGB(loadSurf -> format, 255, 255, 255));
                ghostblocks = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                loadSurf = IMG_Load("Assets\\images\\next.png");
                SDL_SetColorKey(loadSurf, SDL_TRUE, SDL_MapRGB(loadSurf -> format, 255, 255, 255));
                next = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                loadSurf = IMG_Load("Assets\\images\\main_menu.png");
                main_menu = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                loadSurf = IMG_Load("Assets\\images\\game_over.png");
                game_over_bg = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);

                playing = true;
                takeHighScore();
                nextTetrimino(next_items);
                updateFallingTetris();



        fontScore = loadFont("Assets\\Font\\eurof55.ttf", 30);
        fontPause = loadFont("Assets\\Font\\eurof55.ttf", 100);
        fontGameOver = loadFont("Assets\\Font\\Bodie MF Holly.ttf", 50);


        loadMusic("Assets\\Music\\ingame.mp3");


        moveBlock = loadSound("Assets/Sound/move.wav");
        rotateBlock = loadSound("Assets/Sound/rotate.wav");
        hardDrop = loadSound("Assets/Sound/harddrop.wav");
        lineClear = loadSound("Assets/Sound/Lineclear_.wav");
        gameOver = loadSound("Assets/Sound/gameover.wav");

}


TTF_Font* InGame::loadFont(const char* path, int size)
    {
        TTF_Font* gFont = TTF_OpenFont( path, size );
        if (gFont == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                           "Load font %s", TTF_GetError());
        }
        return gFont;
    }

SDL_Texture* InGame::renderText(const std::string &text, TTF_Font* font, SDL_Color textColor)
{
        SDL_Surface* textSurface = TTF_RenderText_Solid( font, text.c_str(), textColor );
        if( textSurface == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"Render text surface %s", TTF_GetError());
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface( render, textSurface );
        if( texture == nullptr ) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,SDL_LOG_PRIORITY_ERROR,"Create texture from text %s", SDL_GetError());
        }
        SDL_FreeSurface(textSurface);
        return texture;
}


void InGame::renderTexture(SDL_Texture *texture, int x, int y,SDL_Renderer* renderer)
{
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}



Mix_Music* InGame::loadMusic(const char* path)
{
        gMusic = Mix_LoadMUS(path);
        if (gMusic == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
                "Could not load music! SDL_mixer Error: %s", Mix_GetError());
        }
        return gMusic;
}


void InGame::playMusic(Mix_Music *gMusic)
{
        if (gMusic == nullptr) return;

        if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic( gMusic, -1 );
        }
        else if( Mix_PausedMusic() == 1 ) {
            Mix_ResumeMusic();
        }
}



Mix_Chunk* InGame::loadSound(const char* path)
{
        Mix_Chunk *gChunk = Mix_LoadWAV(path);
        if (gChunk == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION,
                           SDL_LOG_PRIORITY_ERROR,
               "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
        }
        return gChunk;
}

void InGame::playSound(Mix_Chunk* gChunk)
{
    if (gChunk != nullptr) {
            Mix_PlayChannel( -1, gChunk, 0 );
    }

}


void InGame::waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(100);
    }
}

#endif // UTIL_H
