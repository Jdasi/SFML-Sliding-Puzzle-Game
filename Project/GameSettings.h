#pragma once

#include "GameUnlock.h"
#include "Coordinate.h"

#include <vector>

class GameSettings
{
public:
    GameSettings() = default;
    ~GameSettings() = default;

    static int getImageID();

    static std::string getImageName();
    static void setImageName(const std::string &name, int id);
    
    static Coordinate getSegments();
    static void setSegmentsX(int x);
    static void setSegmentsY(int y);

    static void enumeratePuzzles();
    static std::vector<std::string> getPuzzles();
    static void clearPuzzles();

    static int getCurrentPuzzleValue();
    static void setCurrentPuzzleValue(int value);

    static std::string getFontName();
    static void setFontName(const std::string &str);

    static bool isInitialised();
    static void setInitialised(bool b);

private:
    static std::string imageName;
    static int imageID;
    static Coordinate segments;
    static int currentPuzzleValue;
    static std::vector<std::string> puzzles;
    static std::string fontName;
    static bool initialised;
};