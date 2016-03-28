#pragma once

#include "cocos2d.h"

class GameUnlock
{
public:
    GameUnlock();
    ~GameUnlock() = default;

    std::string getName();
    void setName(std::string str);

    bool isLocked();
    void setLocked(bool lock);

    int getStarCost();
    void setStarCost(int cost);

private:
    std::string name;
    bool locked;
    int starCost;
};