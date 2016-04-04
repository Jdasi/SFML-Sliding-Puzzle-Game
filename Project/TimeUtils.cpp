#include <string>

std::string calculateTime(const std::string &str)
{
    int totalTime = stoi(str);

    int hours = (totalTime / 60) / 60;
    int minutes = (totalTime / 60) - (hours * 60);
    int seconds = totalTime % 60;

    std::string timePlayed =
        std::to_string(hours / 10) + std::to_string(hours % 10) + ":" +
        std::to_string(minutes / 10) + std::to_string(minutes % 10) + ":" +
        std::to_string(seconds / 10) + std::to_string(seconds % 10);

    return timePlayed;
}