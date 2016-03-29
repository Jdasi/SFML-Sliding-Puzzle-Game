#include "GameUnlock.h"

GameUnlock::GameUnlock()
    : id(0)
    , locked(true)
    , starCost(0)
{
}

GameUnlock::GameUnlock
    (const std::string &str, int cost, std::vector<GameUnlock> &container, int iid)
    : name(str)
    , id(iid)
    , locked(true)
    , starCost(cost)
{
    container.push_back(*this);
}

std::string GameUnlock::getName()
{
    return name;
}

void GameUnlock::setName(const std::string &str)
{
    name = str;
}

int GameUnlock::getID()
{
    return id;
}

void GameUnlock::setID(int value)
{
    id = value;
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