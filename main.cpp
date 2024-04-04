#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Tetris.h"
#include <ctime>
#include <SDL_mixer.h>

using namespace std;


int main(int argc, char* argv[])
{
    srand(time(0));
    Tetris *tetris = new Tetris();
    tetris -> init();
        while(tetris -> isRunning())
        {
            tetris -> setCurrentTime(SDL_GetTicks());
            tetris -> handleEvents();
            tetris -> gameplay();
            tetris -> gameover();
            tetris -> updateRender();
        }

    tetris -> clean();
    delete tetris;
    return 0;
}

