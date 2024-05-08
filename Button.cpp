#include "Button.h"
#include <iostream>
#include <stdlib.h>



LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_MOUSE_OUT;
}


void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e )
{
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {

        int x, y;
        SDL_GetMouseState( &x, &y );
        bool inside = true;


        if( x < mPosition.x )
            inside = false;

        else if( x > mPosition.x + BUTTON_WIDTH )
            inside = false;


        else if( y < mPosition.y )
            inside = false;

        else if( y > mPosition.y + BUTTON_HEIGHT )
            inside = false;

        if(!inside)
		{
		   mCurrentSprite = BUTTON_MOUSE_OUT;
		}else{
            if(e -> type == SDL_MOUSEBUTTONDOWN){
                mCurrentSprite = BUTTON_MOUSE_DOWN;
            }
		}

    }
}

void LButton::render(SDL_Texture* texture, SDL_Rect *src, SDL_Rect *dest, SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, src, dest);
}


LButtonSprite LButton::getCurrentSprite()
{
    return mCurrentSprite;
}
void LButton::setCurrentSprite(LButtonSprite sprite)
{
    mCurrentSprite = sprite;
}
