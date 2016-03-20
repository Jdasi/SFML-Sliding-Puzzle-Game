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
    GameSettings() = default;
    ~GameSettings() = default;

    static std::string getImageName();
    static void setImageName(std::string name);
    
    static coordinate getSegments();
    static void setSegmentsX(int x);
    static void setSegmentsY(int y);

    static bool isInitialised();
    static void setInitialised(bool b);

private:
    static std::string imageName;
    static coordinate segments;
    static bool initialised;
};