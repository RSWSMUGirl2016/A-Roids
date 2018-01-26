//*******************************************************************
// Time.hpp
//	A simple high-precision time utility function for Windows
//	based on code by Squirrel Eiserloh
#pragma once
#include <string>
#include <ctime>


//*******************************************************************
double GetCurrentTimeSeconds();

//*******************************************************************
void SleepSeconds(float secondsToSleep);


//*******************************************************************
// Based on Code by Christopher Forseth

#if !defined( __ITW_TIME_TIME__ )
#define __ITW_TIME_TIME__ 

#include "Engine/EngineCommon.hpp"

// High performance timer
uint64_t __fastcall TimeGetOpCount();
uint __fastcall TimeGet_ms();
uint __fastcall TimeGet_us();
double __fastcall TimeGetSeconds();

uint64_t TimeOpCountTo_us(uint64_t op_count);
double TimeOpCountTo_ms(uint64_t op_count);
uint64_t TimeOpCountFrom_ms(double ms);

// NOT THREAD SAFE
char const* TimeOpCountToString(uint64_t op_count);


std::string GetCurrentDateAndTime();
uint64_t __fastcall GetCurrentPerformanceCounter();
double __fastcall PerformanceCounterToSeconds();
#endif