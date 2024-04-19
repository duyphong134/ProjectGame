#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>

#include "Button.h"

const char* WINDOW_TITLE = "Tetris";

enum GameStatus {
    MENU,
    HELP,
    QUIT,
    PLAYING,
    PAUSE,
    GAME_OVER,
    RECORD_SCORE
};

class Graphics {
private:
    enum { SCREEN_WIDTH = 600, SCREEN_HEIGHT = 960 };
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    SDL_Texture* menuTexture = nullptr, *help = nullptr, *help_background = nullptr, *help_exit = nullptr;

    SDL_Rect help_dest;
    SDL_Rect playButtonSrc_rect[BUTTON_SPRITE_TOTAL], helpButtonSrc_rect[BUTTON_SPRITE_TOTAL], levelButtonSrc_rect[BUTTON_SPRITE_TOTAL], helpExitButtonSrc_rect[BUTTON_SPRITE_TOTAL];
    LTexture texturePlay, textureHelp, textureHelpExit;
    LButton playButton, helpButton,help_exitButton, levelButton;
    LButton replay, home;
    InGame *ingame = nullptr;
    GameStatus gameState;

    int level = 1;

public:
    Graphics() {
        if (!init()) {
            std::cerr << "Failed to initialize SDL" << std::endl;
            // Handle initialization failure
            // For example, throw an exception or exit the program
        }
        if(!prepareMenu() || !prepareHelp()){
            std::cout << "Failed to prepare!\n";
        }
        gameState = MENU;
    }

    ~Graphics() {}

    bool prepareMenu() {
        menuTexture = loadTexture("images/main_menu.png");

        if(!texturePlay.loadFromFile("images/game_button_play.png", renderer)){
            std::cout <<  "Failed to load play button sprite texture!\n";
            return false;
        }else{
            playButtonSrc_rect[0].x = 0;
            playButtonSrc_rect[0].y = 0;
            playButtonSrc_rect[0].w = BUTTON_WIDTH;
            playButtonSrc_rect[0].h = BUTTON_HEIGHT;
            for(int i = 1; i < BUTTON_SPRITE_TOTAL; i++)
            {
                playButtonSrc_rect[i].x = 0;
                playButtonSrc_rect[i].y = playButtonSrc_rect[i - 1].y + BUTTON_HEIGHT + 19;
                playButtonSrc_rect[i].w = BUTTON_WIDTH;
                playButtonSrc_rect[i].h = BUTTON_HEIGHT;
            }
            playButton.setPosition(180, 415);
        }

        if(!textureHelp.loadFromFile("images/game_button_help.png", renderer)){
            std::cout <<  "Failed to load help button sprite texture!\n";
            return false;
        }else{
            helpButtonSrc_rect[0].x = 0;
            helpButtonSrc_rect[0].y = 0;
            helpButtonSrc_rect[0].w = BUTTON_WIDTH ;
            helpButtonSrc_rect[0].h = BUTTON_HEIGHT;
            for(int i = 1; i < BUTTON_SPRITE_TOTAL; i++)
            {
                helpButtonSrc_rect[i].x = 0;
                helpButtonSrc_rect[i].y = playButtonSrc_rect[i - 1].y + BUTTON_HEIGHT + 19;
                helpButtonSrc_rect[i].w = BUTTON_WIDTH;
                helpButtonSrc_rect[i].h = BUTTON_HEIGHT;
            }
            helpButton.setPosition(180, 420 + 100);
        }

        return true;
    }

    void handleEventsMenu() {
        SDL_Event e;
        while(gameState == MENU){
           while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    gameState = QUIT;
                    exit(0);
                }
                playButton.handleEvent(&e);
                helpButton.handleEvent(&e);

                if(playButton.getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN){
                    gameState = PLAYING;
                    ingame = new InGame;
                }else if(helpButton.getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN){
                    gameState = HELP;
                }
                renderMenu();
            }
        }
    }

    void renderMenu() {
        prepareScene();
        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
        playButton.render(texturePlay, playButtonSrc_rect, renderer, playButton.getCurrentSprite());
        helpButton.render(textureHelp, helpButtonSrc_rect, renderer, helpButton.getCurrentSprite());

        presentScene();
    }

    bool prepareHelp(){
        help = loadTexture("images/Help_inside.png");
        help_background = loadTexture("images/main_menu2.png");
        if (!textureHelpExit.loadFromFile("images/game_button_exit.png", renderer)){
            std::cout <<  "Failed to load play button sprite texture!\n";
            return false;
        }else{
            helpExitButtonSrc_rect[0].x = 0;
            helpExitButtonSrc_rect[0].y = 0;
            helpExitButtonSrc_rect[0].w = BUTTON_WIDTH ;
            helpExitButtonSrc_rect[0].h = BUTTON_HEIGHT;
            for(int i = 1; i < BUTTON_SPRITE_TOTAL; i++)
            {
                helpExitButtonSrc_rect[i].x = 0;
                helpExitButtonSrc_rect[i].y = helpExitButtonSrc_rect[i - 1].y + BUTTON_HEIGHT + 19;
                helpExitButtonSrc_rect[i].w = BUTTON_WIDTH;
                helpExitButtonSrc_rect[i].h = BUTTON_HEIGHT;
            }
            help_exitButton.setPosition(0, 0);
        }
        return true;
    }

    void renderHelp(){
        prepareScene();
        SDL_RenderCopy(renderer, help_background, NULL, NULL);
        SDL_RenderCopy(renderer, help, NULL, &help_dest);
        help_exitButton.render(textureHelpExit, helpExitButtonSrc_rect, renderer, help_exitButton.getCurrentSprite());

        presentScene();
    }

    void handleEventsHelp(){
        SDL_Event e;
        while(gameState == HELP){
           while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    gameState = QUIT;
                    exit(0);
                }
                help_exitButton.handleEvent(&e);

                if(help_exitButton.getCurrentSprite() == BUTTON_SPRITE_MOUSE_DOWN){
                    gameState = MENU;
                }
                renderHelp();
            }
        }
    }

    void handleEvents() {
        if (gameState == MENU) {
            while(gameState == MENU){
                handleEventsMenu();
            }
        }else if(gameState == PLAYING){
            ingame -> init(window,renderer);
            ingame -> gameLoop();
            if(ingame -> gameover()){
                gameState = GAME_OVER;
            }
        }else if(gameState == GAME_OVER){
                gameState = MENU;
                delete ingame;
        }else if(gameState == HELP){
            while(gameState == HELP){
                renderHelp();
                handleEventsHelp();
            }
        }

    }

    void logErrorAndExit(const char* msg, const char* error) {
        std::cerr << msg << ": " << error << std::endl;
        quit();
    }

    bool init() {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
            logErrorAndExit("SDL_Init", SDL_GetError());
            return false;
        }

        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            logErrorAndExit("CreateWindow", SDL_GetError());
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            logErrorAndExit("CreateRenderer", SDL_GetError());
            return false;
        }

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

        if (TTF_Init() == -1) {
            logErrorAndExit("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
            return false;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            logErrorAndExit("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return false;
        }

        return true;
    }

    void prepareScene(){
        SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
        SDL_RenderClear( renderer );
    }

    void presentScene() {
        SDL_RenderPresent(renderer);
    }

    SDL_Texture* loadTexture(const char* filename) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);

        SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
        if (texture == nullptr) {
            SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
        }
        return texture;
    }

    void renderTexture(SDL_Texture* texture, int x, int y) {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

        SDL_RenderCopy(renderer, texture, nullptr, &dest);
    }

    void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y) {
        SDL_Rect dest;

        dest.x = x;
        dest.y = y;
        dest.w = src->w;
        dest.h = src->h;

        SDL_RenderCopy(renderer, texture, src, &dest);
    }

    void quit() {
        delete ingame;
        ingame = nullptr;
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

#endif // _GRAPHICS__H
