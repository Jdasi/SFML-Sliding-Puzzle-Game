#include "SimpleTimer.h"
#include "GameProfile.h"

SimpleTimer::SimpleTimer()
    : start()
    , end()
{
}

void SimpleTimer::startTimer()
{
    start = std::chrono::steady_clock::now();
}

void SimpleTimer::endTimer()
{
    end = std::chrono::steady_clock::now();
}

void SimpleTimer::endTimerAndRecord()
{
    end = std::chrono::steady_clock::now();
    recordTime();
}

time_t SimpleTimer::getTime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}

void SimpleTimer::recordTime() const
{
    GameProfile::modifyProfileStat(ProfileStat::timePlayed, getTime());
}