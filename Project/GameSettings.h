#pragma once

#include "Coordinate.h"

#include <vector>

class GameSettings
{
public:
    GameSettings() = default;
    ~GameSettings() = default;

    static int getImageID();

    static std::string getImageName();
    static void setImageName(const std::string &name, const int id);
    
    static Coordinate getSegments();
    static void setSegmentsX(const int x);
    static void setSegmentsY(const int y);

    static void enumeratePuzzles();
    static std::vector<std::string> &getPuzzles();
    static void clearPuzzles();

    static int getCurrentPuzzleValue();
    static void setCurrentPuzzleValue(const int value);

    static std::string getFontName();
    static void setFontName(const std::string &str);

    static bool isInitialised();
    static void setInitialised(const bool b);

private:
    static std::string imageName;
    static int imageID;
    static Coordinate segments;
    static int currentPuzzleValue;
    static std::vector<std::string> puzzles;
    static std::string fontName;
    static bool initialised;
};