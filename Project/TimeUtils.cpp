#include <string>

namespace Utility
{

std::string timeToString(const int time)
{
    int hours = (time / 60) / 60;
    int minutes = (time / 60) - (hours * 60);
    int seconds = time % 60;

    std::string timePlayed =
        std::to_string(hours / 10) + std::to_string(hours % 10) + ":" +
        std::to_string(minutes / 10) + std::to_string(minutes % 10) + ":" +
        std::to_string(seconds / 10) + std::to_string(seconds % 10);

    return timePlayed;
}

std::string timeToString(const std::string &str)
{
    int time = stoi(str);
    return timeToString(time);
}

} // Namespace Utility.