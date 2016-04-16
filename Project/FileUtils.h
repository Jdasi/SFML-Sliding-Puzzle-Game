#pragma once

#include <vector>

namespace Utility
{

std::wstring getExecutablePath();
std::vector<std::string> enumerateFiles(const std::wstring &path);
void loadProfile();
void saveProfile();

};