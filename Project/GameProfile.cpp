#include "GameProfile.h"

GameProfile::keymap GameProfile::profileSettings;
std::string GameProfile::currentBackground;
bool GameProfile::initialised = false;
std::vector<GameUnlock> GameProfile::unlocks;

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
            throw std::runtime_error("Error in profileSettingToString");
        }
    }
}

std::string GameProfile::getProfileStat(ProfileStat setting)
{
    return profileSettings[profileSettingToString(setting)];
}

void GameProfile::setProfileStat(ProfileStat setting, const std::string &val)
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

void GameProfile::setCurrentBackground(const std::string &str)
{
    currentBackground = str;
}

void GameProfile::enumerateUnlocks()
{
    GameUnlock regal("regal", 0, unlocks, 0);
    GameUnlock nature("nature", 4, unlocks, 1);
    GameUnlock spooky("spooky", 8, unlocks, 2);
    GameUnlock alien("alien", 16, unlocks, 3);

    std::string unlockedBackgrounds = getProfileStat(ProfileStat::backgroundsUnlocked);

    for (GameUnlock g : unlocks)
    {
        if (unlockedBackgrounds.find(g.getName()) != std::string::npos)
        {
            unlocks[g.getID()].setLocked(false);
        }
    }
}

std::vector<GameUnlock> &GameProfile::getUnlocks()
{
    return unlocks;
}

int GameProfile::stringToBackgroundID(const std::string &str)
{
    for (GameUnlock g : unlocks)
    {
        if (str == g.getName())
        {
            return g.getID();
        }
    }

    throw std::runtime_error("Error in stringToBackgroundID");
}

bool GameProfile::isInitialised()
{
    return initialised;
}

void GameProfile::setInitialised(bool b)
{
    initialised = b;
}