#include "GameProfile.h"

GameProfile::keymap GameProfile::profileSettings;
std::string GameProfile::currentBackground;
bool GameProfile::initialised = false;

GameProfile::keymap &GameProfile::getProfileSettings()
{
   return profileSettings;
}

std::string GameProfile::getProfileSetting(std::string setting)
{
    return profileSettings[setting];
}

void GameProfile::setProfileSetting(std::string setting, std::string val)
{
    profileSettings[setting] = val;
}

void GameProfile::modifyProfileSetting(std::string setting, int amount)
{
    int tempVal = stoi(profileSettings[setting]) + amount;
    profileSettings[setting] = std::to_string(tempVal);
}

std::string GameProfile::getCurrentBackground()
{
    return currentBackground;
}

void GameProfile::setCurrentBackground(std::string str)
{
    currentBackground = str;
}

bool GameProfile::isInitialised()
{
    return initialised;
}

void GameProfile::setInitialised(bool b)
{
    initialised = b;
}