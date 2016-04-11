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
    static std::string profileStatToString(const ProfileStat &setting);

    static std::string getProfileStat(const ProfileStat &setting);
    static void setProfileStat(const ProfileStat &setting, const std::string &val);
    static void modifyProfileStat(const ProfileStat &setting, int amount);
    static void modifyProfileStat(const ProfileStat &setting, 
                                  const std::string &addition);

    static std::string getCurrentBackground();

    static void enumerateUnlocks();
    static std::vector<GameUnlock> &getUnlocks();

    static int stringToBackgroundID(const std::string &str);

    static bool isInitialised();
    static void setInitialised(const bool b);
    
private:
    static keymap profileStats;
    static bool initialised;
    static std::vector<GameUnlock> unlocks;
};