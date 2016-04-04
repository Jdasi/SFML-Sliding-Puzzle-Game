#pragma once

#include <string>
#include <vector>

class GameUnlock
{
public:
    GameUnlock(const std::string &str, int cost, std::vector<GameUnlock> &container, 
               int iid);
    ~GameUnlock() = default;

    std::string getName() const;
    void setName(const std::string &str);

    int getID() const;
    void setID(int value);

    bool isLocked() const;
    void setLocked(bool lock);

    int getStarCost() const;
    void setStarCost(int cost);

private:
    std::string name;
    int id;
    bool locked;
    int starCost;
};