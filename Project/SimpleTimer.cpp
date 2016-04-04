#include "SimpleTimer.h"
#include "GameProfile.h"

SimpleTimer::SimpleTimer()
    : start()
    , end()
    , recorded(false)
{
}

void SimpleTimer::startTimer()
{
    start = std::chrono::steady_clock::now();
}

void SimpleTimer::makeTimePoint()
{
    endTimer();
}

void SimpleTimer::endTimer()
{
    end = std::chrono::steady_clock::now();
}

void SimpleTimer::endTimerAndRecord()
{
    endTimer();
    recordTime();
}

time_t SimpleTimer::getTime() const
{
    return std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
}

void SimpleTimer::recordTime()
{
    if (recorded)
    {
        return;
    }

    GameProfile::modifyProfileStat(ProfileStat::timePlayed, getTime());
    recorded = true;
}