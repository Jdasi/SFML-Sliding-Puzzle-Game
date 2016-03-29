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

    if (unlockedBackgrounds.find("regal") != std::string::npos)
    {
        unlocks[backgroundNameToInt(BackgroundName::regal)].setLocked(false);
    }

    if (unlockedBackgrounds.find("nature") != std::string::npos)
    {
        unlocks[backgroundNameToInt(BackgroundName::nature)].setLocked(false);
    }

    if (unlockedBackgrounds.find("spooky") != std::string::npos)
    {
        unlocks[backgroundNameToInt(BackgroundName::spooky)].setLocked(false);
    }

    if (unlockedBackgrounds.find("alien") != std::string::npos)
    {
        unlocks[backgroundNameToInt(BackgroundName::alien)].setLocked(false);
    }
}

std::vector<GameUnlock> &GameProfile::getUnlocks()
{
    return unlocks;
}

int GameProfile::backgroundNameToInt(BackgroundName name)
{
    return static_cast<int>(name);
}

bool GameProfile::isInitialised()
{
    return initialised;
}

void GameProfile::setInitialised(bool b)
{
    initialised = b;
}