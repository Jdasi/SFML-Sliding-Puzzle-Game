#pragma once

#include "GameUnlock.h"

#include <map>

enum class ProfileStat
{
    stars,
    timePlayed,
    totalMoves,
    totalStarsCollected,
    puzzlesAttempted,
    puzzlesCompleted,
    backgroundsUnlocked,
    currentBackground,
    animatedShuffling
};

class GameProfile
{
public:
    GameProfile() = default;
    ~GameProfile() = default;

    using Keymap = std::map<std::string, std::string>;

    static Keymap &getProfileKeymap();
    static std::string profileStatToString(const ProfileStat &setting);

    static std::string getProfileStat(const ProfileStat &setting);
    static void setProfileStat(const ProfileStat &setting, const std::string &value);
    static void setProfileStat(const ProfileStat &setting, const bool value);
    static void modifyProfileStat(const ProfileStat &setting, int amount);
    static void modifyProfileStat(const ProfileStat &setting, 
                                  const std::string &addition);

    static std::string getCurrentBackground();
    static bool animatedShufflingEnabled();

    static void enumerateUnlocks();
    static std::vector<GameUnlock> &getUnlocks();

    static int stringToBackgroundID(const std::string &str);

    static bool isInitialised();
    static void setInitialised(const bool b);
    
private:
    static Keymap profileStats;
    static bool initialised;
    static std::vector<GameUnlock> unlocks;
};