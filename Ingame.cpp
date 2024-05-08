#include "Util.h"
#include <cstring>
#include <ctime>
#include <set>
#include <fstream>

/*
    0   1   2   3
    4   5   6   7
*/
const int InGame::figures[7][4] =
{
     0, 1, 2, 3,    // I
     0, 4, 5, 6,    // J
     2, 6, 5, 4,    // L
     1, 2, 5, 6,    // O
     2, 1, 5, 4,    // S
     1, 4, 5, 6,    // T
     0, 1, 5, 6,    // z
};


void InGame::nextTetrimino(Point things[])
{
    srand(time(0));
	static std::set<int> colorsBag;
	if(colorsBag.begin() == colorsBag.end()){
        for(int i = 1; i < 7; i++){
            colorsBag.insert(i);
        }
	}
	auto it1 = colorsBag.begin();
	advance(it1, rand()%colorsBag.size());
	nextColor = *it1;

    static std::set<int> tetradsBag;
    if(tetradsBag.begin() == tetradsBag.end()){
        for(int i = 0; i < 7; i++){
            tetradsBag.insert(i);
        }
    }
    auto it2 = tetradsBag.begin();
    advance(it2, rand()%tetradsBag.size());
    int n = *it2;


	// Chuyển về tọa độ xOy
	for (int i = 0; i < 4; i++)
	{
		things[i].x = (figures[n][i] % 4) + ((Cols / 2) - 2);
		things[i].y = int(figures[n][i] / 4);
	}

	colorsBag.erase(colorsBag.find(nextColor));
    tetradsBag.erase(tetradsBag.find(n));
}

void InGame::updateFallingTetris()
{
    for(int i = 0; i < 4; i++){
        items[i].x = next_items[i].x;
        items[i].y = next_items[i].y;
    }
    color = nextColor;
    nextTetrimino(next_items);
}

void InGame::handleEventsInGame()
{

    const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_DOWN])
		delay = 50;


	SDL_Event e;
	while (SDL_PollEvent(&e))
	{=
		switch (e.type)
		{
		case SDL_QUIT:
			playing = false;
			exit(0);
			break;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
            case SDLK_ESCAPE:
                pauseTexture = renderText("PAUSE", fontPause, textColorPause);
                renderTexture(pauseTexture, 135, 400, render);
                SDL_RenderPresent(render);
                 if( Mix_PausedMusic() == 1 ){
                    Mix_ResumeMusic();
                }else{
                    Mix_PauseMusic();
                }
                waitUntilKeyPressed();
                break;
			case SDLK_UP:
			    playSound(rotateBlock);
				rotate = true;
				break;
            case SDLK_z:
                playSound(rotateBlock);
				rotate = true;
				break;
			case SDLK_LEFT:
			    playSound(moveBlock);
				dx = -1;
				break;
			case SDLK_RIGHT:
			    playSound(moveBlock);
				dx = 1;
				break;
            case SDLK_SPACE:
                playSound(hardDrop);
                while(isValid(items)){
                    for(int i= 0; i < 4; i++){
                        items[i].y++;
                    }
                }
                for(int i = 0; i < 4; i++){
                    items[i].y--;
                }

			default:
				break;
			}
		default:
			break;
		}
	}
}

void InGame::setRectPos(SDL_Rect& rect, int x, int y, int w, int h)
{
	rect = { x, y, w, h };
}

void InGame::moveRectPos(SDL_Rect& rect, int x, int y)
{
	rect.x += x;
	rect.y += y;
}

bool InGame::isValid(const Point things[])
{
	for (int i = 0; i < 4; i++)
		if (things[i].x < 0 || things[i].x >= Cols || things[i].y >= Lines)
			return false;
		else if (field[things[i].y][things[i].x])
			return false;
	return true;
}


void InGame::animateClearedRows(int rowIndex) {
    const int animationDuration = 500;

    const int numFrames = 50;

    int rIncrement = (255 / numFrames);
    int gIncrement = (255 / numFrames);
    int bIncrement = (255 / numFrames);

    int r = 0, g = 0, b = 0;

    for (int frame = 0; frame < numFrames; frame++) {
        SDL_SetRenderDrawColor(render, r, g, b, SDL_ALPHA_OPAQUE);

        for (int j = 0; j < Cols; j++) {
            SDL_Rect blockRect;

            setRectPos(blockRect, j * BlockW + BlockW, rowIndex * BlockH + ScreenH - (Lines + 1) * BlockH, BlockW, BlockH);

            SDL_RenderFillRect(render, &blockRect);
        }

        SDL_RenderPresent(render);

        r += rIncrement;
        g += gIncrement;
        b += bIncrement;

        SDL_Delay(animationDuration / numFrames);
    }
}


void InGame::removeClearedRows(int rowIndex) {
    playSound(lineClear);
    for (int i = rowIndex; i > 0; i--) {
        for (int j = 0; j < Cols; j++) {
            field[i][j] = field[i - 1][j];
        }
    }
    for (int j = 0; j < Cols; j++) {
        field[0][j] = 0;
    }
}


void InGame::gameplay()
{

	for (int i = 0; i < 4; i++)
		backup[i] = items[i];

    for(int i = 0; i < 4; i++)
        ghostblock[i] = items[i];

    while(isValid(ghostblock)){
        for(int i = 0; i < 4; i++){
            ghostblock[i].y++;
        }
    }


    for(int i = 0; i < 4; i++)
    {
       ghostbackup[i] = ghostblock[i];
    }


	if (dx)
	{
		for (int i = 0; i < 4; i++)
		{
			items[i].x += dx;
		}
		if (!isValid(items))
			for (int i = 0; i < 4; i++)
				items[i] = backup[i];
        if(!isValid(ghostblock))
            for(int i = 0; i < 4; i++)
                ghostblock[i] = ghostbackup[i];
    }

	if (rotate)
	{
		Point p = items[2];
		for (int i = 0; i < 4; i++)
		{
			int x = items[i].y - p.y;
			int y = items[i].x - p.x;
			items[i].x = p.x - x;
			items[i].y = p.y + y;
		}
		if (!isValid(items))
			for (int i = 0; i < 4; i++)
				items[i] = backup[i];
        if(!isValid(ghostblock))
            for(int i = 0; i < 4; i++)
                ghostblock[i] = ghostbackup[i];
	}

	if (currentTime - startTime >delay)
	{

		for (int i = 0; i < 4; i++)
			backup[i] = items[i];
		for (int i = 0; i < 4; i++)
			items[i].y++;

		if (!isValid(items))
		{
			for (int i = 0; i < 4; i++)
				field[backup[i].y][backup[i].x] = color;
            updateFallingTetris();
			nextTetrimino(next_items);
		}

		startTime = currentTime;
	}

	int k = Lines - 1;
	for (int i = k; i >= 0; i--)
	{
		int  cnt = 0;
		for (int j = 0; j < Cols; j++)
		{
			if (field[i][j])
				cnt++;
		}
		if (cnt < Cols){
            k--;
		}else if(cnt == Cols){
		    animateClearedRows(i);
		    removeClearedRows(i);
            currentPoint += 100;

		}

	}
	dx = 0;
	rotate = false;
	delay = 300;

}

bool InGame::gameover()
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
    if(lines_cnt > 20)
    {
        playSound(gameOver);
        if(currentPoint > highScore){
            highScore = currentPoint;
            updateHighScore();
        }
        playing = false;
        return true;
    }

    return false;
}

void InGame::renderGameOver()
{
    std::string strHighScore = std::to_string(highScore);
    std::string strPoint = std::to_string(currentPoint);
    SDL_RenderClear(render);
    SDL_RenderCopy(render, game_over_bg, NULL, NULL);

    gameOverHighScore = renderText(strHighScore, fontGameOver, textColorGameOver);
    renderTexture(gameOverHighScore, 170, 450, render);

    gameOverScore = renderText(strPoint, fontGameOver, textColorGameOver);
    renderTexture(gameOverScore , 380, 450, render);

    SDL_RenderPresent(render);
}

void InGame::updateRender()
{
    std::string strPoint = std::to_string(currentPoint);
    SDL_RenderClear(render);
	SDL_RenderCopy(render, background, NULL, NULL);
	SDL_RenderCopy(render, scoreframe, NULL, &dest_score);
	SDL_RenderCopy(render, next, NULL, &dest_next);

	fontTexture = renderText(strPoint, fontScore , textColorScore);
	renderTexture(fontTexture, 75, 100, render);

	for (int i = 0; i < 4; i++)
	{
		setRectPos(srcR, nextColor * BlockW);
		setRectPos(destR, next_items[i].x * BlockW, next_items[i].y * BlockH);
		moveRectPos(destR, 450 - 110, 320);
		SDL_RenderCopy(render, blocks, &srcR, &destR);
	}

	for (int i = 0; i < Lines; i++)
		for (int j = 0; j < Cols; j++)
			if (field[i][j])
			{
				setRectPos(srcR, field[i][j] * BlockW);
				setRectPos(destR, j * BlockW, i * BlockH);
				moveRectPos(destR, BlockW, ScreenH - (Lines+ 1) * BlockH);
				SDL_RenderCopy(render, blocks, &srcR, &destR);
			}
	for (int i = 0; i < 4; i++)
	{
		setRectPos(srcR, color * BlockW);
		setRectPos(destR, items[i].x * BlockW, items[i].y * BlockH);
		moveRectPos(destR, BlockW, ScreenH - (Lines + 1) * BlockH);
		SDL_RenderCopy(render, blocks, &srcR, &destR);
	}

	for (int i = 0; i < 4; i++)
    {
        setRectPos(srcR, color * 36);
        setRectPos(destR, ghostblock[i].x * BlockW, ghostblock[i].y * BlockH);
        moveRectPos(destR, BlockW, ScreenH - (Lines + 1) * BlockH - BlockH);
        SDL_RenderCopy(render, ghostblocks, &srcR, &destR);
    }


	SDL_RenderPresent(render);

}

void InGame::takeHighScore()
{
    std::fstream file("highscore.txt");
    file >> highScore;
    file.close();
}

void InGame::updateHighScore()
{
    std::fstream file("highscore.txt");
    file << highScore;
    file.close();
}

void InGame::clean()
{
	SDL_DestroyTexture(blocks);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(render);
	IMG_Quit();
	SDL_Quit();
}

void InGame::replay()
{
    for (int i = 0; i < Lines; i++) {
        for (int j = 0; j < Cols; j++) {
            field[i][j] = 0;
        }
    }

    currentPoint = 0;
    playing = true;
    delay = 300;
    startTime = SDL_GetTicks();

    nextTetrimino(next_items);
    nextTetrimino(items);
    color = nextColor;
    nextTetrimino(next_items);

    updateRender();
}

void InGame::gameLoop()
{
    while(isPlaying())
        {
            playMusic(gMusic);
            setCurrentTime(SDL_GetTicks());
            handleEventsInGame();
            gameplay();
            gameover();
            updateRender();
        }
}


