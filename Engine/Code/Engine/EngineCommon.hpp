#pragma once

#include <atomic>
#include <stdint.h>
#include <stdio.h>
#include <conio.h>

#define COMBINE1(X,Y) X##Y
#define COMBINE(X,Y) COMBINE1(X,Y)

typedef unsigned int uint;
typedef unsigned __int8 byte_t;

// used in a sample
extern std::atomic<int> gA;
extern int gB;

void Pause();

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);


struct uint3
{
    uint x;
    uint y;
    uint z;
};
