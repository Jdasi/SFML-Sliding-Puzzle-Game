#include "GameProfile.h"

GameProfile::keymap GameProfile::profileSettings;
std::string GameProfile::currentBackground;
bool GameProfile::initialised = false;

GameProfile::keymap &GameProfile::getProfileKeymap()
{
   return profileSettings;
}

std::string GameProfile::profileSettingToString(ProfileStat setting)
{
    switch (setting)
    {
        case ProfileStat::stars:
        {
            return "stars";
        }
        case ProfileStat::timePlayed:
        {
            return "timePlayed";
        }
        case ProfileStat::totalMoves:
        {
            return "totalMoves";
        }
        case ProfileStat::puzzlesAttempted:
        {
            return "puzzlesAttempted";
        }
        case ProfileStat::puzzlesCompleted:
        {
            return "puzzlesCompleted";
        }
        case ProfileStat::backgroundsUnlocked:
        {
            return "backgroundsUnlocked";
        }
        case ProfileStat::currentBackground:
        {
            return "currentBackground";
        }
        default:
        {
            return "null";
        }
    }
}

std::string GameProfile::getProfileStat(ProfileStat setting)
{
    return profileSettings[profileSettingToString(setting)];
}

void GameProfile::setProfileStat(ProfileStat setting, std::string val)
{
    profileSettings[profileSettingToString(setting)] = val;
}

void GameProfile::modifyProfileStat(ProfileStat setting, int amount)
{
    std::string actualSetting = profileSettingToString(setting);
    int tempVal = stoi(profileSettings[actualSetting]) + amount;
    profileSettings[actualSetting] = std::to_string(tempVal);
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