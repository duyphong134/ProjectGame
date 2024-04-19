#include "GameState.h"

GameState::GameState()
{
    status = MENU;
}

GameState::~GameState()
{
}

bool GameState::isRunning()
{
    return status != GAME_OVER;
}

bool GameState::isOver()
{
    return status == GAME_OVER;
}

bool GameState::isPause()
{
    return status == PAUSE;
}

bool GameState::getRecord()
{
    return status == RECORD_SCORE;
}

GameStatus GameState::setStatus(GameStatus newStatus)
{
    status = newStatus;
    return status;
}
