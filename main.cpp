#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "InGame.h"
#include "Graphics.h"
#include <SDL_mixer.h>


int main(int argc, char* argv[])
{
    Graphics* tetris = new Graphics;
    while(true)
    {
        tetris -> handleEvents();
    }
    tetris -> quit();
    delete tetris;
    tetris = nullptr;
    return 0;
}



