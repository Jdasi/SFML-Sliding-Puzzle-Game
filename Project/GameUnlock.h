#pragma once

#include "cocos2d.h"

class GameUnlock
{
public:
    GameUnlock();
    GameUnlock
        (const std::string &str, int cost, std::vector<GameUnlock> &container, int iid);

    ~GameUnlock() = default;

    std::string getName();
    void setName(const std::string &str);

    int getID();
    void setID(int value);

    bool isLocked();
    void setLocked(bool lock);

    int getStarCost();
    void setStarCost(int cost);

private:
    std::string name;
    int id;
    bool locked;
    int starCost;
};