#include "Project/GameSettings.h"

std::string GameSettings::imageName;
coordinate GameSettings::segments { 0, 0 };

GameSettings::GameSettings()
{
}

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

void GameSettings::setSegments(int x, int y)
{
    segments.x = x;
    segments.y = y;
}