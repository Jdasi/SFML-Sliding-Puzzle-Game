#include "Project/GameSettings.h"

std::string GameSettings::imageName;
coordinate GameSettings::segments { 0, 0 };
bool GameSettings::initialised = false;

std::string GameSettings::getImageName()
{
    return imageName;
}

void GameSettings::setImageName(std::string name)
{
    imageName = name;
}

coordinate GameSettings::getSegments()
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

bool GameSettings::isInitialised()
{
    return initialised;
}

void GameSettings::setInitialised(bool b)
{
    initialised = b;
}