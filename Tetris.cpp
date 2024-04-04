#include "Tetris.h"
#include <iostream>
const char* title = "Tetris";

/*
    0   1   2   3
    4   5   6   7
*/
const int Tetris::figures[7][4] =
{
     0, 1, 2, 3,    // I
     0, 4, 5, 6,    // J
     2, 6, 5, 4,    // L
     1, 2, 5, 6,    // O
     2, 1, 5, 4,    // S
     1, 4, 5, 6,    // T
     0, 1, 5, 6,    // z
};
void Tetris::logErrorAndExit(const char* msg, const char* error)
{
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "%s: %s", msg, error);
    SDL_Quit();
}
void Tetris::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenW, ScreenH, SDL_WINDOW_RESIZABLE);
        //full screen
        //window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))
            logErrorAndExit( "SDL_image error:", IMG_GetError());

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
                                              SDL_RENDERER_PRESENTVSYNC);

    if (render == NULL) logErrorAndExit("CreateRenderer", SDL_GetError());
    else{
                SDL_SetRenderDrawColor(render, 255, 255, 255, 255);
                int imgFlags = IMG_INIT_PNG;
                int initted = IMG_Init(imgFlags);
                if((initted & imgFlags) != imgFlags)
                    std::cout << "Failed to init required png support\n" << "IMG_Init() Error : " << IMG_GetError() << std::endl;
                SDL_Surface* loadSurf = IMG_Load("images\\background.png");
                background = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                loadSurf = IMG_Load("images\\blocks.png");
                blocks = SDL_CreateTextureFromSurface(render, loadSurf);
                SDL_FreeSurface(loadSurf);
                running = true;
                nextTetrimino();
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(render, ScreenW, ScreenH);
}

void Tetris::nextTetrimino()
{
	color = 1 + rand() % 7; // hàm random màu
	int n = rand() % 7; // hàm random khối tetris

	// Chuyển về tọa độ xOy
	for (int i = 0; i < 4; i++)
	{
		items[i].x = figures[n][i] % 4;
		items[i].y = int(figures[n][i] / 4);
	}
}

void Tetris::handleEvents()
{
    // tách hẳn trường hợp SDLK_DOWN ra để giảm độ trễ của chương trình

    const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_DOWN])
		delay = 50;


	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_UP:
				rotate = true;
				break;
			case SDLK_LEFT:
				dx = -1;
				break;
			case SDLK_RIGHT:
				dx = 1;
				break;

			default:
				break;
			}
		default:
			break;
		}
	}
}

void Tetris::setRectPos(SDL_Rect& rect, int x, int y, int w, int h)
{
	rect = { x, y, w, h };
}

void Tetris::moveRectPos(SDL_Rect& rect, int x, int y)
{
	rect.x += x;
	rect.y += y;
}

bool Tetris::isValid()
{
	for (int i = 0; i < 4; i++)
		if (items[i].x < 0 || items[i].x >= Cols || items[i].y >= Lines) // Kiểm tra xem vị trí của khối liệu có ra ngoài khỏi khung của trờ chơi hay ko
			return false;
		else if (field[items[i].y][items[i].x]) // Kiểm tra xem liệu có chạm vào một block đã có sẵn chưa
			return false;
	return true;
}

void Tetris::gameplay()
{

	////////// backup
	for (int i = 0; i < 4; i++)
		backup[i] = items[i];
	////////// di chuyển
	if (dx)
	{
		for (int i = 0; i < 4; i++)
		{
			items[i].x += dx;
		}
		if (!isValid())
			for (int i = 0; i < 4; i++)
				items[i] = backup[i];
    }

	///////// xoay
	if (rotate)
	{
		Point p = items[2];	// trục quay
		for (int i = 0; i < 4; i++)
		{
			int x = items[i].y - p.y;
			int y = items[i].x - p.x;
			items[i].x = p.x - x;
			items[i].y = p.y + y;
		}
		if (!isValid())
			for (int i = 0; i < 4; i++)
				items[i] = backup[i];
	}
	///////// tick

	if (currentTime - startTime >delay)
	{
        // Nếu như currentTime - startTime > delay nghĩa là block di chuyển không ra ngoài khung và cũng chưa di chuyển tới dưới cùng của grid
		for (int i = 0; i < 4; i++)
			backup[i] = items[i];
		for (int i = 0; i < 4; i++)
			items[i].y++;
        // Nếu như tại đây mà ko valid nghĩa là block đã chạm đến đ
		if (!isValid())
		{
			for (int i = 0; i < 4; i++)
				field[backup[i].y][backup[i].x] = color;
			nextTetrimino();
		}

		startTime = currentTime;
	}

	//////// check lines
	int k = Lines - 1;
	for (int i = k; i > 0; i--)
	{
		int  cnt = 0;
		for (int j = 0; j < Cols; j++)
		{
			if (field[i][j])
				cnt++;
			field[k][j] = field[i][j];
		}
		if (cnt < Cols)
			k--;
	}
	dx = 0;
	rotate = false;
	delay = 300;

}

bool Tetris::gameover()
{
    int lines_cnt = 0;
    for(int i = Lines - 1; i >= 0; i--){
        for(int j = 0; j < Cols; j++)
        {
            if(field[i][j]){
                lines_cnt++;
                break;
            }
        }
    }
    if(lines_cnt == Lines)
        running = false;
        return true;

    return false;
}


void Tetris::updateRender()
{
    SDL_RenderClear(render);
	SDL_RenderCopy(render, background, NULL, NULL);
	for (int i = 0; i < Lines; i++)
		for (int j = 0; j < Cols; j++)
			if (field[i][j])
			{
				setRectPos(srcR, field[i][j] * BlockW);
				setRectPos(destR, j * BlockW, i * BlockH);
				moveRectPos(destR, BlockW, ScreenH - (Lines + 1) * BlockH);
				SDL_RenderCopy(render, blocks, &srcR, &destR);
			}
	for (int i = 0; i < 4; i++)
	{
		setRectPos(srcR, color * BlockW);
		setRectPos(destR, items[i].x * BlockW, items[i].y * BlockH);
		moveRectPos(destR, BlockW, ScreenH - (Lines + 1) * BlockH);
		SDL_RenderCopy(render, blocks, &srcR, &destR);
	}

	SDL_RenderPresent(render);

}


void Tetris::clean()
{
	SDL_DestroyTexture(blocks);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(render);
	IMG_Quit();
	SDL_Quit();
}
