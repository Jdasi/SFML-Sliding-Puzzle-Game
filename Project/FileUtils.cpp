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

std::wstring get_executable_path()
{
    const char executeableName[] = "PuzzleGame.exe";

    TCHAR path[MAX_PATH];

    // Check if the file path is too long.
    if (GetModuleFileName(nullptr, path, MAX_PATH - 1) == 0)
    {
        auto errorcode = GetLastError();
        throw std::runtime_error("Could not resolve runtime path: Error " + std::to_string(errorcode));
    }

    std::wstring pathstr = path;

    // Return the path minus EXECUTABLE_NAME.
    return pathstr.substr(0, pathstr.length()-sizeof(executeableName));
}

std::vector<std::string> enumerate_files(const std::wstring& path)
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = FindFirstFile(path.c_str(), &ffd);

    switch (GetLastError())
    {
        case ERROR_SUCCESS: break; // Everything is a'ok.
        case ERROR_FILE_NOT_FOUND: return {}; // No files were found in the folder
        default:
            throw std::runtime_error("Error in FindFirstFile");
    }

    std::vector<std::string> files;
    do
    {
        if (GetLastError() != ERROR_SUCCESS)
            throw std::runtime_error("Error in FindNextFile");

        // If the current file is not a directory.
        if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            std::wstring fileName = ffd.cFileName;
            files.push_back(wstring_to_string(fileName));
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    
    if (GetLastError() != ERROR_NO_MORE_FILES)
        throw std::runtime_error("Expected no more files at the end of the loop.");

    if(FindClose(hFind) == 0)
        throw std::runtime_error("Error in FindClose");

    return files;
}

void loadProfile()
{
    std::ifstream file("profile.txt");

    if (!file.is_open())
        throw std::runtime_error("File not found");

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