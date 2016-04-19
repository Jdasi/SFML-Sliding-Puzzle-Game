#include "GameProfile.h"
#include "GameExceptions.h"

#include <string>

GameProfile::Keymap GameProfile::profileStats;
bool GameProfile::initialised = false;
const int GameProfile::numUnlocks = 4;
GameUnlock GameProfile::unlocks[numUnlocks];

GameProfile::Keymap &GameProfile::getProfileKeymap()
{
    return profileStats;
}

std::string GameProfile::profileStatToString(const ProfileStat &setting)
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
        case ProfileStat::animatedShuffling:
        {
            return "animatedShuffling";
        }
        default:
        {
            throw Utility::ProfileStatConversionException();
        }
    }
}

std::string GameProfile::getProfileStat(const ProfileStat &setting)
{
    return profileStats[profileStatToString(setting)];
}

void GameProfile::setProfileStat(const ProfileStat &setting, const std::string &value)
{
    profileStats[profileStatToString(setting)] = value;
}

void GameProfile::setProfileStat(const ProfileStat &setting, const bool value)
{
    if (setting != ProfileStat::animatedShuffling)
    {
        throw Utility::ProfileStatConversionException();
    }

    std::string actualValue;
    switch (value)
    {
        case true:
        {
            actualValue = "true";
            break;
        }
        case false:
        {
            actualValue = "false";
            break;
        }
        default:
        {
            throw Utility::ProfileStatConversionException();
        }
    }
    
    setProfileStat(setting, actualValue);
}

void GameProfile::modifyProfileStat(const ProfileStat &setting, int amount)
{
    if (setting > ProfileStat::puzzlesCompleted)
    {
        throw Utility::ProfileStatConversionException();
    }

    std::string actualSetting = profileStatToString(setting);
    int tempVal = stoi(profileStats[actualSetting]) + amount;
    profileStats[actualSetting] = std::to_string(tempVal);
}

void GameProfile::modifyProfileStat(const ProfileStat &setting,
                                    const std::string &addition)
{
    if (setting != ProfileStat::backgroundsUnlocked)
    {
        throw Utility::ProfileStatConversionException();
    }

    std::string actualSetting = profileStatToString(setting);
    profileStats[actualSetting] += " " + addition;
}

std::string GameProfile::getCurrentBackground()
{
    return profileStats[profileStatToString(ProfileStat::currentBackground)];
}

bool GameProfile::animatedShufflingEnabled()
{
    std::string shuffleStr = getProfileStat(ProfileStat::animatedShuffling);

    if (shuffleStr == "true")
    {
        return true;
    }

    if (shuffleStr == "false")
    {
        return false;
    }

    throw Utility::ProfileStatConversionException();
}

// In a finished game the unlocks would be read from a file. For now they are hard coded.
void GameProfile::enumerateUnlocks()
{
    GameUnlock regal("regal", 0, unlocks, 0);
    GameUnlock nature("nature", 4, unlocks, 1);
    GameUnlock spooky("spooky", 8, unlocks, 2);
    GameUnlock alien("alien", 12, unlocks, 3);

    std::string unlockedBackgrounds = getProfileStat(ProfileStat::backgroundsUnlocked);
    for (GameUnlock &g : unlocks)
    {
        if (unlockedBackgrounds.find(g.getName()) != std::string::npos)
        {
            unlocks[g.getID()].setLocked(false);
        }
    }
}

int GameProfile::getNumUnlocks()
{
    return numUnlocks;
}

GameUnlock *GameProfile::getUnlocks()
{
    return unlocks;
}

int GameProfile::stringToBackgroundID(const std::string &str)
{
    for (GameUnlock &g : unlocks)
    {
        if (str == g.getName())
        {
            return g.getID();
        }
    }

    throw Utility::BackgroundConversionException();
}

bool GameProfile::isInitialised()
{
    return initialised;
}

void GameProfile::setInitialised(const bool b)
{
    initialised = b;
}