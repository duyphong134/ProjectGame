
#include <string>
#include <SDL.h>
#include <SDL_image.h>

const int BUTTON_WIDTH = 210;
const int BUTTON_HEIGHT = 80;
const int TOTAL_BUTTONS = 3;

enum LButtonSprite
{
    BUTTON_MOUSE_DOWN = 0,
    BUTTON_MOUSE_OUT = 1
};


class LButton
{
    public:
        LButton();

        void setPosition( int x, int y );

        void handleEvent( SDL_Event* e );

        void render(SDL_Texture* texture, SDL_Rect *src, SDL_Rect *dest, SDL_Renderer* renderer);

        LButtonSprite getCurrentSprite();
        void setCurrentSprite(LButtonSprite sprite);

    private:
        SDL_Point mPosition;

        LButtonSprite mCurrentSprite;
};




