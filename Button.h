
#include <string>
#include <SDL.h>
#include <SDL_image.h>

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




