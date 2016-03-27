#pragma once

#include "GameSettings.h"
#include "GameUnlock.h"

#include <string>
#include <vector>

enum class ProfileSetting
{
    stars = 0,
    timePlayed,
    totalMoves,
    puzzlesAttempted,
    puzzlesCompleted,
    favouritePuzzle,
    hardestPuzzleSolved,
    backgroundsUnlocked
};

class GameProfile
{
public:
    using keymap = std::map<std::string, std::string>;

    static keymap &getProfileSettings();

    static std::string getProfileSetting(std::string setting);
    static void setProfileSetting(std::string setting, std::string val);
    static void modifyProfileSetting(std::string setting, int amount);

    static std::string getCurrentBackground();
    static void setCurrentBackground(std::string str);

    static bool isInitialised();
    static void setInitialised(bool b);
    
private:
    static keymap profileSettings;
    static std::string currentBackground;
    static bool initialised;
};