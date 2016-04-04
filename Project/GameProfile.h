#pragma once

#include "GameUnlock.h"

#include <map>

enum class ProfileStat
{
    stars = 0,
    timePlayed,
    totalMoves,
    totalStarsCollected,
    puzzlesAttempted,
    puzzlesCompleted,
    backgroundsUnlocked,
    currentBackground
};

class GameProfile
{
public:
    using keymap = std::map<std::string, std::string>;

    static keymap &getProfileKeymap();
    static std::string profileStatToString(ProfileStat setting);

    static std::string getProfileStat(ProfileStat setting);
    static void setProfileStat(ProfileStat setting, const std::string &val);
    static void modifyProfileStat(ProfileStat setting, int amount);
    static void modifyProfileStat(ProfileStat setting, const std::string &addition);

    static std::string getCurrentBackground();

    static void enumerateUnlocks();
    static std::vector<GameUnlock> &getUnlocks();

    static int stringToBackgroundID(const std::string &str);

    static bool isInitialised();
    static void setInitialised(bool b);
    
private:
    static keymap profileStats;
    static bool initialised;
    static std::vector<GameUnlock> unlocks;
};