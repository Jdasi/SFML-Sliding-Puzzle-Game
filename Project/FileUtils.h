#pragma once

#include <vector>

std::wstring getExecutablePath();
std::vector<std::string> enumerateFiles(const std::wstring &path);
void loadProfile();
void saveProfile();