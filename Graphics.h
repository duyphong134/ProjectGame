#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <fstream>

#include "Button.h"

const char* WINDOW_TITLE = "Tetris";


enum GameStatus {
    MENU,
    HELP,
    QUIT,
    PLAYING,
    PAUSE,
    GAME_OVER,
    SETTING
};

const int SLIDER_WIDTH = 200;
const int SLIDER_HEIGHT = 5;
const int SLIDER_X = 190;
const int SLIDER_Y = 115;

const int PADDING_WIDTH = 10;
const int PADDING_HEIGHT = 20;

int sliderValue = 0;
bool isDragging = false;

class Graphics {
private:
    enum { SCREEN_WIDTH = 600, SCREEN_HEIGHT = 960 };
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
    SDL_Texture* menuTexture = nullptr;

    SDL_Rect rect;
    SDL_Rect playButton_dest, helpButton_dest, helpButtonExit_dest, settingButton_dest;
    LButton playButton, helpButton, help_exitButton, settingButton;
    SDL_Texture *helpBackground = NULL, *settingBackground = NULL;
    InGame *ingame = new InGame();
    GameStatus gameState;
    SDL_Event e;

    int volumeLevel = 0;

public:
    Graphics() {
        if (!init()) {
            std::cerr << "Failed to initialize SDL" << std::endl;
        }

        menuTexture = loadTexture("Assets/images/main_menu22.png");
        helpBackground = loadTexture("Assets/images/help2.png");
        settingBackground = loadTexture("Assets/images/nice2.png");

        playButton.setPosition(193, 412);
        helpButton.setPosition(193, 505);
        settingButton.setPosition(193, 590);

        ingame -> init(window, renderer);
        gameState = MENU;
    }

    ~Graphics() {}

    void renderMenu() {
        prepareScene();
        SDL_RenderCopy(renderer, menuTexture, NULL, NULL);
        presentScene();
    }

    void handleEventsMenu() {
        while(gameState == MENU){
           while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    gameState = QUIT;
                    exit(0);
                }
                playButton.handleEvent(&e);
                helpButton.handleEvent(&e);
                settingButton.handleEvent(&e);

                renderMenu();

                if(playButton.getCurrentSprite() == BUTTON_MOUSE_DOWN){
                    gameState = PLAYING;
                    ingame -> replay();
                    ingame -> gameLoop();
                    return;
                }else if(helpButton.getCurrentSprite() == BUTTON_MOUSE_DOWN){
                    gameState = HELP;
                    return;
                }else if(settingButton.getCurrentSprite() == BUTTON_MOUSE_DOWN){
                    gameState = SETTING;
                    return;
                }
            }
        }
        gameState = MENU;
    }

    void renderHelp(){
        prepareScene();
        SDL_RenderCopy(renderer, helpBackground, NULL, NULL);
        presentScene();
    }

    void handleEventsHelp(){
        while(gameState == HELP){
           while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    gameState = QUIT;
                    exit(0);
                }
                help_exitButton.handleEvent(&e);


                if(help_exitButton.getCurrentSprite() == BUTTON_MOUSE_DOWN){
                    gameState = MENU;
                }
            }
        }
    }

    void handleEventsGameOver(){
        while(gameState == GAME_OVER){
            while(SDL_PollEvent(&e) != 0){
                int x = e.motion.x;
                int y = e.motion.y;
                    if(e.type == SDL_QUIT){
                        exit(0);
                    }else if(e.type == SDL_MOUSEBUTTONDOWN){
                        if((x > 188 && x < 235) && (y > 540 && y < 580)){
                            gameState = MENU;
                        }else if((x > 259 && x < 322) && (y > 540 && y < 580)){
                            ingame -> replay();
                            gameState = PLAYING;
                        }

                }
            }
        }
    }

    void renderSetting() {
        prepareScene();
        SDL_RenderCopy(renderer, settingBackground, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect trackRect = {SLIDER_X, SLIDER_Y, SLIDER_WIDTH, SLIDER_HEIGHT};
        SDL_RenderFillRect(renderer, &trackRect);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int handleX = SLIDER_X + sliderValue;
        SDL_Rect handleRect = {handleX, SLIDER_Y - 10, PADDING_WIDTH, PADDING_HEIGHT};
        SDL_RenderFillRect(renderer, &handleRect);

        presentScene();
    }


    void handleEventsSetting()
    {
        while(gameState == SETTING){
            while(SDL_PollEvent(&e) != 0){
                std:: cerr << e.motion.x << " " << e.motion.y << std::endl;
                if(e.type == SDL_QUIT){
                    exit(0);
                }else if (e.type ==  SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    gameState = MENU;
                    return;
                }else if(e.type == SDL_MOUSEBUTTONDOWN){
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    if(mouseX >= SLIDER_X && mouseX <= SLIDER_X + SLIDER_WIDTH && mouseY >= 100 && mouseY <= 130){
                        isDragging = true;
                    }
                }else if(e.type == SDL_MOUSEBUTTONUP){
                    isDragging = false;
                }else if(e.type == SDL_MOUSEMOTION){
                    if(isDragging){
                        int mouseX, mouseY;
                        SDL_GetMouseState(&mouseX, &mouseY);
                        sliderValue = mouseX - SLIDER_X;
                        if (sliderValue < 0) sliderValue = 0;
                        if (sliderValue > SLIDER_WIDTH) sliderValue = SLIDER_WIDTH;
                        volumeLevel = (sliderValue) / SLIDER_WIDTH * 128;
                        Mix_Volume(-1, volumeLevel);
                    }
                }
            }
                renderSetting();
        }
    }

    int getSoundLevelSetting()
    {
            return volumeLevel;
    }

    void handleEvents() {
        if (gameState == MENU) {
                while(gameState == MENU){
                    handleEventsMenu();
                }
        }else if(gameState == PLAYING){
            ingame -> gameLoop();
            if(ingame -> gameover()){
                gameState = GAME_OVER;
            }
        }else if(gameState == GAME_OVER){
                ingame -> renderGameOver();
                handleEventsGameOver();
        }else if(gameState == HELP){
            while(gameState == HELP){
                renderHelp();
                handleEventsHelp();
            }
        }else if(gameState == SETTING){
                    handleEventsSetting();
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
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
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
