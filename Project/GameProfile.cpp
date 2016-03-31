#include "GameProfile.h"

GameProfile::keymap GameProfile::profileStats;
bool GameProfile::initialised = false;
std::vector<GameUnlock> GameProfile::unlocks;

GameProfile::keymap &GameProfile::getProfileKeymap()
{
    return profileStats;
}

std::string GameProfile::profileStatToString(ProfileStat setting)
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
        case ProfileStat::totalStarsCollected:
        {
            return "totalStarsCollected";
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
            throw std::runtime_error("Error in profileStatToString");
        }
    }
}

std::string GameProfile::getProfileStat(ProfileStat setting)
{
    return profileStats[profileStatToString(setting)];
}

void GameProfile::setProfileStat(ProfileStat setting, const std::string &val)
{
    profileStats[profileStatToString(setting)] = val;
}

void GameProfile::modifyProfileStat(ProfileStat setting, int amount)
{
    std::string actualSetting = profileStatToString(setting);
    int tempVal = stoi(profileStats[actualSetting]) + amount;
    profileStats[actualSetting] = std::to_string(tempVal);
}

void GameProfile::modifyProfileStat(ProfileStat setting, const std::string &addition)
{
    std::string actualSetting = profileStatToString(setting);
    profileStats[actualSetting] += " " + addition;
}

std::string GameProfile::getCurrentBackground()
{
    return profileStats[profileStatToString(ProfileStat::currentBackground)];
}

void GameProfile::enumerateUnlocks()
{
    GameUnlock regal("regal", 0, unlocks, 0);
    GameUnlock nature("nature", 2, unlocks, 1);
    GameUnlock spooky("spooky", 4, unlocks, 2);
    GameUnlock alien("alien", 8, unlocks, 3);

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