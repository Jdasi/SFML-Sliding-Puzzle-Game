#include "GameUnlock.h"

GameUnlock::GameUnlock
    (const std::string &str, int cost, std::vector<GameUnlock> &container, int iid)
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

void GameUnlock::setID(int value)
{
    id = value;
}

bool GameUnlock::isLocked() const
{
    return locked;
}

void GameUnlock::setLocked(bool lock)
{
    locked = lock;
}

int GameUnlock::getStarCost() const
{
    return starCost;
}

void GameUnlock::setStarCost(int cost)
{
    starCost = cost;
}