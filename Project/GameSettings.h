#pragma once
#include <string>

struct coordinate
{
    coordinate(int argX, int argY)
        : x(argX)
        , y(argY)
    {
    }

    int x;
    int y;
};

class GameSettings
{
public:
    GameSettings();
    ~GameSettings() = default;

    static std::string getImageName();
    static void setImageName(std::string name);
    
    static coordinate getSegments();
    static void setSegments(int x, int y);

private:
    static std::string imageName;
    static coordinate segments;
};