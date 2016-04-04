#include "GameSettings.h"
#include "FileUtils.h"

std::string GameSettings::imageName;
int GameSettings::imageID = 0;
Coordinate GameSettings::segments { 0, 0 };
int GameSettings::currentPuzzleValue = 0;
bool GameSettings::initialised = false;
std::vector<std::string> GameSettings::puzzles;

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

void GameSettings::setSegmentsX(int x)
{
    segments.x = x;
}

void GameSettings::setSegmentsY(int y)
{
    segments.y = y;
}

void GameSettings::enumeratePuzzles()
{
    std::wstring executable_path = getExecutablePath();

    std::vector<std::string> puzzlesJpg = 
        enumerateFiles(executable_path + L"\\puzzles\\*.jpg");
    std::vector<std::string> puzzlesPng =
        enumerateFiles(executable_path + L"\\puzzles\\*.png");

    puzzles.reserve(puzzlesJpg.size() + puzzlesPng.size());
    puzzles.insert(puzzles.end(), puzzlesJpg.begin(), puzzlesJpg.end());
    puzzles.insert(puzzles.end(), puzzlesPng.begin(), puzzlesPng.end());

    if (puzzles.size() == 0)
    {
        throw std::runtime_error("Puzzles folder is empty");
    }
}

std::vector<std::string> GameSettings::getPuzzles()
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

void GameSettings::setCurrentPuzzleValue(int value)
{
    currentPuzzleValue = value;
}

bool GameSettings::isInitialised()
{
    return initialised;
}

void GameSettings::setInitialised(bool b)
{
    initialised = b;
}