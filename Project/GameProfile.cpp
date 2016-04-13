#include "GameProfile.h"

GameProfile::keymap GameProfile::profileStats;
bool GameProfile::initialised = false;
std::vector<GameUnlock> GameProfile::unlocks;

GameProfile::keymap &GameProfile::getProfileKeymap()
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
            throw std::runtime_error("Error in profileStatToString");
        }
    }
}

std::string GameProfile::getProfileStat(const ProfileStat &setting)
{
    return profileStats[profileStatToString(setting)];
}

void GameProfile::setProfileStat(const ProfileStat &setting, const std::string &val)
{
    profileStats[profileStatToString(setting)] = val;
}

void GameProfile::modifyProfileStat(const ProfileStat &setting, int amount)
{
    std::string actualSetting = profileStatToString(setting);
    int tempVal = std::stoi(profileStats[actualSetting]) + amount;
    profileStats[actualSetting] = std::to_string(tempVal);
}

void GameProfile::modifyProfileStat(const ProfileStat &setting,
                                    const std::string &addition)
{
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

    throw std::runtime_error("error in animatedShufflingEnabled");
}

// In a finished game the Unlocks would be read from a file. For now they are hard coded.
void GameProfile::enumerateUnlocks()
{
    GameUnlock regal("regal", 0, unlocks, 0);
    GameUnlock nature("nature", 4, unlocks, 1);
    GameUnlock spooky("spooky", 8, unlocks, 2);
    GameUnlock alien("alien", 12, unlocks, 3);

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

void GameProfile::setInitialised(const bool b)
{
    initialised = b;
}