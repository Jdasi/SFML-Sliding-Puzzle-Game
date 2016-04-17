#pragma once

#include <chrono>

class SimpleTimer
{
public:
    SimpleTimer();
    ~SimpleTimer() = default;

    void startTimer();
    void makeTimePoint();
    void endTimerAndRecord();
    time_t getTime() const;

private:
    void endTimer();
    void recordTime();

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    bool recorded;
};