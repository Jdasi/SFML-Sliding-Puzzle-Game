#include "GameUnlock.h"

GameUnlock::GameUnlock(const std::string &str, const int cost, 
                       std::vector<GameUnlock> &container, const int iid)
    : name(str)
    , id(iid)
    , locked(true)
    , starCost(cost)
{
    container.push_back(*this);
}

std::string GameUnlock::getName() const
{
    return name;
}

void GameUnlock::setName(const std::string &str)
{
    name = str;
}

int GameUnlock::getID() const
{
    return id;
}

void GameUnlock::setID(const int value)
{
    id = value;
}

bool GameUnlock::isLocked() const
{
    return locked;
}

void GameUnlock::setLocked(const bool lock)
{
    locked = lock;
}

int GameUnlock::getStarCost() const
{
    return starCost;
}

void GameUnlock::setStarCost(const int cost)
{
    starCost = cost;
}