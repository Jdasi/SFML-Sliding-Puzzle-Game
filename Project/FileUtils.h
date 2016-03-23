#pragma once

#include <string>
#include <vector>

std::wstring get_executable_path();
std::vector<std::string> enumerate_files(const std::wstring& path);