#pragma once

#include "GameUnlock.h"

#include <vector>

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

    static int getImageID();

    static std::string getImageName();
    static void setImageName(const std::string &name, int id);
    
    static coordinate getSegments();
    static void setSegmentsX(int x);
    static void setSegmentsY(int y);

    static void enumeratePuzzles();
    static std::vector<std::string> getPuzzles();

    static int getCurrentPuzzleValue();
    static void setCurrentPuzzleValue(int value);

    static bool isInitialised();
    static void setInitialised(bool b);

private:
    static std::string imageName;
    static int imageID;
    static coordinate segments;
    static int currentPuzzleValue;
    static bool initialised;
    static std::vector<std::string> puzzles;
};