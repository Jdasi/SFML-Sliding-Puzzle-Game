#include "Project/GameSettings.h"

std::string GameSettings::imageName;
int GameSettings::imageID = 0;
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

int GameSettings::getImageID()
{
    return imageID;
}

void GameSettings::setImageName(std::string name, int id)
{
    imageName = name;
    imageID = id;
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