#ifndef _util_H
#define _util_H
#endif // _util_H


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

class util{
private:
    SDL_Window *window;
    SDL_Renderer *render;
    SDL_Rect srcR;
    SDL_Rect destR;
    SDL_Texture *texture;
    enum { ScreenW = 600, ScreenH = 960 };
    const char* title = "Tetris";
public:
    void logErrorAndExit(const char* msg, const char* error);
    void initSDL(SDL_Window *window, SDL_Renderer *render);
    void quitSDL(SDL_Window *window, SDL_Renderer *render);
    void waitUntilKeyPressed();
    SDL_Texture* loadTexture(const char *filename, SDL_Renderer *render);
    void music(const char * path);
    void sound(const char * path);

};

