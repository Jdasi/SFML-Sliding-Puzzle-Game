#include "GameUnlock.h"

GameUnlock::GameUnlock()
    : locked(false)
    , starCost(0)
{
}

std::string GameUnlock::getName()
{
    return name;
}

void GameUnlock::setName(std::string str)
{
    name = str;
}

bool GameUnlock::isLocked()
{
    return locked;
}

void GameUnlock::setLocked(bool lock)
{
    locked = lock;
}

int GameUnlock::getStarCost()
{
    return starCost;
}

void GameUnlock::setStarCost(int cost)
{
    starCost = cost;
}