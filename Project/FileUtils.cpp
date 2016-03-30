#include "GameProfile.h"

#include <windows.h>
#include <vector>
#include <codecvt>
#include <fstream>

// Because filenames are widestrings (accept international characters)
// We need to convert them to regular strings
static std::string wstring_to_string(const std::wstring &str)
{
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type, wchar_t> converter;
    return converter.to_bytes(str);
}

std::wstring getExecutablePath()
{
    const char executeableName[] = "PuzzleGame.exe";

    TCHAR path[MAX_PATH];

    // Check if the file path is too long.
    if (GetModuleFileName(nullptr, path, MAX_PATH - 1) == 0)
    {
        throw std::runtime_error("Could not resolve runtime path.");
    }

    std::wstring pathstr = path;

    // Return the path minus EXECUTABLE_NAME.
    return pathstr.substr(0, pathstr.length()-sizeof(executeableName));
}

std::vector<std::string> enumerateFiles(const std::wstring& path)
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFile(path.c_str(), &ffd);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
            return {};

        throw std::runtime_error("Error in FindFirstFile");
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

    if(FindClose(hFind) == 0)
        throw std::runtime_error("Error in FindClose");

    return files;
}

void saveProfile()
{
    std::ofstream file("profile.txt");

    if (!file.is_open())
        throw std::runtime_error("File not found");

    GameProfile::keymap &profileSettings = GameProfile::getProfileKeymap();
    for (auto itr : profileSettings)
    {
        std::string line = itr.first + "=" + itr.second + '\n';
        file << line;
    }

    file.close();
}

void generateProfile()
{
    GameProfile::keymap &profileSettings = GameProfile::getProfileKeymap();
    profileSettings.clear();

    profileSettings["backgroundsUnlocked"] = "regal";
    profileSettings["currentBackground"] = "regal";
    profileSettings["puzzlesAttempted"] = "0";
    profileSettings["puzzlesCompleted"] = "0";
    profileSettings["stars"] = "0";
    profileSettings["timePlayed"] = "0";
    profileSettings["totalMoves"] = "0";

    saveProfile();
}

void loadProfile()
{
    std::ifstream file("profile.txt");

    // There is no file. Generate one.
    if (!file.is_open())
    {
        generateProfile();
    }

    GameProfile::keymap &profileSettings = GameProfile::getProfileKeymap();

    std::string line;
    while (std::getline(file, line))
    {
        int pos = line.find_first_of('=');

        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);

        profileSettings[key] = val;
    }
}
