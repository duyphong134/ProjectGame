#ifndef INGAME_H_

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


class InGame
{
private:

    enum{ ScreenW = 600, ScreenH = 960};
    enum{ BlockW = 36, BlockH = 36};
    enum{ Lines = 22, Cols = 10};


    SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;
    SDL_Texture* loadSurf = NULL;


    SDL_Texture* background = NULL, *blocks = NULL, *ghostblocks = NULL, *scoreframe = NULL, *next = NULL;


    SDL_Texture* main_menu = NULL;
    SDL_Texture* game_over_bg = NULL;


    SDL_Rect srcR = {0, 0, BlockW, BlockH}, destR = {0, 0, BlockW, BlockH};
    SDL_Rect dest_score = {0, 0, 165, 165};
    SDL_Rect dest_next = {435, 168, 167, 335};


    bool playing= false;


    int field[Lines][Cols]= {0};
    static const int figures[7][4];
    struct Point
    {
        int x, y;
    } next_items[4],items[4], backup[4], ghostblock[4], ghostbackup[4];
    int color = 1, nextColor = 1;
    int currentPoint = 0, highScore;
    int dx = 0;
    bool rotate = false;
    unsigned int delay = 300;
    Uint32  startTime = 0, currentTime = 0;


    TTF_Font *fontScore = NULL;
    SDL_Color textColorScore = {240, 66, 225, 255};
    SDL_Texture *fontTexture = NULL;

    TTF_Font *fontPause = NULL;
    SDL_Color textColorPause = {0, 0, 0, 255};
    SDL_Texture *pauseTexture = NULL;

    TTF_Font *fontGameOver = NULL;
    SDL_Color textColorGameOver = {255, 255, 255, 255};
    SDL_Texture *gameOverHighScore = NULL;
    SDL_Texture *gameOverScore = NULL;


    int soundLevel = 0;
    Mix_Music *gMusic = NULL;
    Mix_Chunk *moveBlock = NULL;
    Mix_Chunk *rotateBlock = NULL;
    Mix_Chunk *hardDrop = NULL;
    Mix_Chunk *lineClear = NULL;
    Mix_Chunk *gameOver = NULL;


public:
    InGame()
    {
    }
    ~InGame()
    {
    }

    void setCurrentTime(Uint32 t)
    {
        currentTime = t;
    }
    bool isPlaying()
    {
        if(!playing){
            Mix_HaltMusic();
        }
        return playing;
    }
    void logErrorAndExit(const char* msg, const char* error);


    bool isValid(const Point []);
    void animateClearedRows(int rowIndex);
    void removeClearedRows(int rowIndex);
    void init(SDL_Window* game_window, SDL_Renderer* game_renderer);
    void nextTetrimino(Point things[]);
    void updateFallingTetris();
    void handleEventsInGame();
    void setRectPos(SDL_Rect &rect, int x = 0, int y = 0, int w = BlockW, int h = BlockH);
    void moveRectPos(SDL_Rect &rect, int x, int y);
    void gameplay();
    bool gameover();
    void renderGameOver();
    void updateRender();
    void takeHighScore();
    void updateHighScore();
    void clean();
    void waitUntilKeyPressed();


    TTF_Font* loadFont(const char* path, int size);
    SDL_Texture* renderText(const std::string &text, TTF_Font* font, SDL_Color textColor);
    void renderTexture(SDL_Texture *texture, int x, int y, SDL_Renderer* renderer);


    Mix_Music *loadMusic(const char* path);
    void playMusic(Mix_Music *gMusic);


    Mix_Chunk* loadSound(const char* path);
    void playSound(Mix_Chunk* gChunk);


    void replay();
    void gameLoop();
};

#endif // INGAME_H_
