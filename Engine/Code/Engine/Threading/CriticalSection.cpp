#include "Engine/Threading/CriticalSection.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//*******************************************************************
CriticalSection::CriticalSection()
{
    ::InitializeCriticalSection(&cs);
}

//*******************************************************************
CriticalSection::~CriticalSection()
{
    ::DeleteCriticalSection(&cs);
}

//*******************************************************************
void CriticalSection::lock()
{
    ::EnterCriticalSection(&cs);
}

//*******************************************************************
bool CriticalSection::try_lock()
{
    return TRUE == ::TryEnterCriticalSection(&cs);
}

//*******************************************************************
void CriticalSection::unlock()
{
    ::LeaveCriticalSection(&cs);
}