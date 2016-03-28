#pragma once

#include "GameUnlock.h"

enum class ProfileStat
{
    stars = 0,
    timePlayed,
    totalMoves,
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
    static std::string profileSettingToString(ProfileStat setting);

    static std::string getProfileStat(ProfileStat setting);
    static void setProfileStat(ProfileStat setting, std::string val);
    static void modifyProfileStat(ProfileStat setting, int amount);

    static std::string getCurrentBackground();
    static void setCurrentBackground(std::string str);

    static bool isInitialised();
    static void setInitialised(bool b);
    
private:
    static keymap profileSettings;
    static std::string currentBackground;
    static bool initialised;
};