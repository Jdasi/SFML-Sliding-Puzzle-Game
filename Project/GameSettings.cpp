#include "GameSettings.h"
#include "FileUtils.h"

std::string GameSettings::imageName;
int GameSettings::imageID = 0;
Coordinate GameSettings::segments { 0, 0 };
int GameSettings::currentPuzzleValue = 0;
std::vector<std::string> GameSettings::puzzles;
std::string GameSettings::fontName;
bool GameSettings::initialised = false;

int GameSettings::getImageID()
{
    return imageID;
}

std::string GameSettings::getImageName()
{
    return imageName;
}

void GameSettings::setImageName(const std::string &name, int id)
{
    imageName = name;
    imageID = id;
}

Coordinate GameSettings::getSegments()
{
    return segments;
}

void GameSettings::setSegmentsX(const int x)
{
    segments.x = x;
}

void GameSettings::setSegmentsY(const int y)
{
    segments.y = y;
}

void GameSettings::enumeratePuzzles()
{
    if (puzzles.size() != 0)
    {
        clearPuzzles();
    }

    std::wstring executable_path = Utility::getExecutablePath();

    std::vector<std::string> puzzlesJpg =
        Utility::enumerateFiles(executable_path + L"\\puzzles\\*.jpg");
    std::vector<std::string> puzzlesPng =
        Utility::enumerateFiles(executable_path + L"\\puzzles\\*.png");

    puzzles.reserve(puzzlesJpg.size() + puzzlesPng.size());
    puzzles.insert(puzzles.end(), puzzlesJpg.begin(), puzzlesJpg.end());
    puzzles.insert(puzzles.end(), puzzlesPng.begin(), puzzlesPng.end());

    if (puzzles.size() == 0)
    {
        throw std::runtime_error("Puzzles folder is empty");
    }
}

std::vector<std::string> &GameSettings::getPuzzles()
{
    return puzzles;
}

void GameSettings::clearPuzzles()
{
    puzzles.clear();
}

int GameSettings::getCurrentPuzzleValue()
{
    return currentPuzzleValue;
}

void GameSettings::setCurrentPuzzleValue(const int value)
{
    currentPuzzleValue = value;
}

std::string GameSettings::getFontName()
{
    return fontName;
}

void GameSettings::setFontName(const std::string& str)
{
    fontName = str;
}

bool GameSettings::isInitialised()
{
    return initialised;
}

void GameSettings::setInitialised(const bool b)
{
    initialised = b;
}