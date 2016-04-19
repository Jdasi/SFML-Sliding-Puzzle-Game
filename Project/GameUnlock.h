#pragma once

#include <string>

class GameUnlock
{
public:
    GameUnlock();
    GameUnlock(const std::string &str, const int cost, GameUnlock container[], 
               const int iid);
    ~GameUnlock() = default;

    std::string getName() const;
    void setName(const std::string &str);

    int getID() const;
    void setID(const int value);

    bool isLocked() const;
    void setLocked(const bool lock);

    int getStarCost() const;
    void setStarCost(const int cost);

private:
    std::string name;
    int id;
    bool locked;
    int starCost;
};