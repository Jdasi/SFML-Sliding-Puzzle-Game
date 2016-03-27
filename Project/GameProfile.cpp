#include "GameProfile.h"

GameProfile::keymap GameProfile::profileSettings;
std::string GameProfile::currentBackground;
bool GameProfile::initialised = false;

GameProfile::keymap &GameProfile::getProfileSettings()
{
   return profileSettings;
}

std::string GameProfile::profileSettingToString(ProfileSetting setting)
{
    switch (setting)
    {
        case ProfileSetting::stars:
        {
            return "stars";
        }
        case ProfileSetting::timePlayed:
        {
            return "timePlayed";
        }
        case ProfileSetting::totalMoves:
        {
            return "totalMoves";
        }
        case ProfileSetting::puzzlesAttempted:
        {
            return "puzzlesAttempted";
        }
        case ProfileSetting::puzzlesCompleted:
        {
            return "puzzlesCompleted";
        }
        case ProfileSetting::backgroundsUnlocked:
        {
            return "backgroundsUnlocked";
        }
        default:
        {
            return "null";
        }
    }
}

std::string GameProfile::getProfileSetting(ProfileSetting setting)
{
    return profileSettings[profileSettingToString(setting)];
}

void GameProfile::setProfileSetting(ProfileSetting setting, std::string val)
{
    profileSettings[profileSettingToString(setting)] = val;
}

void GameProfile::modifyProfileSetting(ProfileSetting setting, int amount)
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