
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include <stdlib.h>

const int BUTTON_WIDTH = 240;
const int BUTTON_HEIGHT = 83;
const int TOTAL_BUTTONS = 3;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

//Texture wrapper class
class LTexture
{
    public:
        //Initializes variables
        LTexture();

        //Deallocates memory
        ~LTexture();

        //Loads image at specified path
        bool loadFromFile(std::string path, SDL_Renderer *renderer);

        //Deallocates texture
        void free();

        //Set color modulation
        void setColor( Uint8 red, Uint8 green, Uint8 blue );

        //Set blending
        void setBlendMode( SDL_BlendMode blending );

        //Set alpha modulation
        void setAlpha( Uint8 alpha );

        //Renders texture at given point
        void render( int x, int y, SDL_Rect clip, SDL_Renderer* renderer = NULL);


        //Gets image dimensions
        int getWidth();
        int getHeight();

    private:
        //The actual hardware texture
        SDL_Texture* mTexture;
        SDL_Renderer* renderer = NULL;


        //Image dimensions
        int mWidth;
        int mHeight;
};

//The mouse button
class LButton
{
    public:
        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e );

        //Shows button sprite
        void render(LTexture gButtonSpriteSheetTexture, SDL_Rect clip[], SDL_Renderer* renderer, int currentSprite);

        LButtonSprite getCurrentSprite();
        void setCurrentSprite(LButtonSprite sprite);

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        LButtonSprite mCurrentSprite;
};


//LTexture

LTexture::LTexture() {
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

bool LTexture::loadFromFile(std::string path, SDL_Renderer* renderer) {
    free();
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        return false;
    }

    mTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    if (mTexture == nullptr) {
        return false;
    }

    mWidth = loadedSurface->w;
    mHeight = loadedSurface->h;

    SDL_FreeSurface(loadedSurface);

    return true;
}

void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
    SDL_SetTextureBlendMode( mTexture, blending );
}

void LTexture::setAlpha( Uint8 alpha )
{
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render( int x, int y, SDL_Rect clip, SDL_Renderer* renderer){
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };
		renderQuad.w = clip.w;
		renderQuad.h = clip.h;
    SDL_RenderCopy(renderer, mTexture, &clip, &renderQuad);
}


int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}


//LButton

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}


void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
     //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }
           //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void LButton::render(LTexture gButtonSpriteSheetTexture, SDL_Rect clip[], SDL_Renderer* renderer, int currentSprite)
{
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, clip[currentSprite] , renderer);
}


LButtonSprite LButton::getCurrentSprite()
{
//    std::cout << mCurrentSprite << std::endl;
    return mCurrentSprite;
}
void LButton::setCurrentSprite(LButtonSprite sprite)
{
    mCurrentSprite = sprite;
}



const char* WINDOW_TITLE = "Tetris";
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
enum{SCREEN_WIDTH = 600, SCREEN_HEIGHT = 960};

enum GameStatus {
    MENU,
    HELP,
    QUIT,
    PLAYING,
    PAUSE,
    GAME_OVER,
    SETTING
};
SDL_Texture* menuTexture = nullptr, *help = nullptr;
SDL_Rect playButtonSrc_rect[BUTTON_SPRITE_TOTAL], helpButtonSrc_rect[BUTTON_SPRITE_TOTAL], settingButtonSrc_rect[BUTTON_SPRITE_TOTAL], helpExitButtonSrc_rect[BUTTON_SPRITE_TOTAL];
LTexture texturePlay, textureHelp, textureHelpExit, textureSetting;
LButton playButton, helpButton,help_exitButton, settingButton, replayButton, homeIngameButton;

void logErrorAndExit(const char* msg, const char* error) {
        std::cerr << msg << ": " << error << std::endl;
        exit(0);
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


int main(int argc, char* argv[])
{
    if(!init){
        std::cout << "dumb\n";
    }else{

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
    }


}
