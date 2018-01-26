#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Core/Time.hpp"

//*******************************************************************
Stopwatch::Stopwatch()
    : m_intervalTime(0.0f)
    , m_targetTime(0.0f)
{}

//*******************************************************************
Stopwatch::~Stopwatch()
{}

//*******************************************************************
void Stopwatch::SetSeconds(float seconds)
{
    m_intervalTime = seconds;
    m_targetTime = (float)GetCurrentTimeSeconds();
}

//*******************************************************************
void Stopwatch::SetFrequency(float hz)
{
    SetSeconds(1.0f / hz);
}

//*******************************************************************
bool Stopwatch::Check()
{
    float currentTime = (float)GetCurrentTimeSeconds();
    return (currentTime >= m_targetTime);
}

//*******************************************************************
bool Stopwatch::CheckAndDecrement()
{
    if (Check())
    {
        m_targetTime += m_intervalTime;
        return true;
    }
    else
    {
        return false;
    }
}

//*******************************************************************
bool Stopwatch::CheckAndReset()
{
    if (Check())
    {
        Reset();
        return true;
    }
    else
    {
        return false;
    }
}

//*******************************************************************
uint Stopwatch::DecrementAll()
{
    uint count = 0;
    while (CheckAndDecrement())
    {
        ++count;
    }
    return count;
}

//*******************************************************************
void Stopwatch::Reset()
{
    m_targetTime = (float)GetCurrentTimeSeconds() + m_intervalTime;
}
