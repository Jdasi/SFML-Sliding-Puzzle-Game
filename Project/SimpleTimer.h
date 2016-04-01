#pragma once

#include <chrono>

class SimpleTimer
{
public:
    SimpleTimer();
    ~SimpleTimer() = default;

    void startTimer();
    void endTimer();
    void endTimerAndRecord();
    time_t getTime() const;
    void recordTime() const;

private:
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};