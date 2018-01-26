#pragma once
#include "Engine/EngineCommon.hpp"

class Stopwatch
{
public:
    float m_intervalTime;
    float m_targetTime;

public:
    Stopwatch();
    ~Stopwatch();

    void SetSeconds(float seconds);
    void SetFrequency(float hz);
    bool Check();
    bool CheckAndDecrement();
    bool CheckAndReset();
    uint DecrementAll();
    void Reset();
};