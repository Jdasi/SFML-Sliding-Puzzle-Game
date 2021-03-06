#include "GameProfile.h"
#include "GameExceptions.h"

#include <windows.h>
#include <vector>
#include <codecvt>
#include <fstream>

namespace Utility
{

/* Because filenames are widestrings (accept international characters)
 * We need to convert them to regular strings.
 */
static std::string wstring_to_string(const std::wstring &str)
{
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.to_bytes(str);
}

std::wstring getExecutablePath()
{
    char executeableName[] = "PuzzleGame.exe";

    TCHAR path[MAX_PATH];

    // Check if the file path is too long.
    if (GetModuleFileName(nullptr, path, MAX_PATH - 1) == 0)
    {
        throw ExecutablePathException();
    }

    std::wstring pathstr = path;

    // Return the path minus executableName.
    return pathstr.substr(0, pathstr.length()-sizeof(executeableName));
}

// Uses Windows API to list files from a directory.
std::vector<std::string> enumerateFiles(const std::wstring& path)
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFile(path.c_str(), &ffd);

    // If there is a problem finding first file of expected type.
    if (hFind == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            return {};
        }

        throw FileEnumerationException();
    }

    std::vector<std::string> files;
    do
    {
        // If the current file is not a directory.
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            std::wstring fileName = ffd.cFileName;
            files.push_back(wstring_to_string(fileName));
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    // If there are no more files.
    if(FindClose(hFind) == 0)
    {
        throw FileEnumerationException();
    }

    return files;
}

void saveProfile()
{
    std::ofstream file("profile.txt");

    if (!file.is_open())
    {
        throw ProfileAccessException();
    }

    GameProfile::Keymap &profileSettings = GameProfile::getProfileKeymap();
    for (auto setting : profileSettings)
    {
        std::string line = setting.first + "=" + setting.second + '\n';
        file << line;
    }

    file.close();
}

void generateProfile()
{
    GameProfile::Keymap &profileSettings = GameProfile::getProfileKeymap();
    profileSettings.clear();

    profileSettings[GameProfile::profileStatToString
        (ProfileStat::backgroundsUnlocked)] = "regal";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::currentBackground)] = "regal";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::puzzlesAttempted)] = "0";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::puzzlesCompleted)] = "0";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::stars)] = "0";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::timePlayed)] = "0";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::totalMoves)] = "0";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::totalStarsCollected)] = "0";
    profileSettings[GameProfile::profileStatToString
        (ProfileStat::animatedShuffling)] = "true";

    saveProfile();
}

void loadProfile()
{
    std::ifstream file("profile.txt");

    if (!file.is_open())
    {
        generateProfile();
    }

    GameProfile::Keymap &profileSettings = GameProfile::getProfileKeymap();

    std::string line;
    while (std::getline(file, line))
    {
        int pos = line.find_first_of('=');

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);

        profileSettings[key] = val;
    }
}

} // Namespace Utility.