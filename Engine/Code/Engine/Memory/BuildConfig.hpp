#pragma once

// Turning Profiler On/Off
#define PROFILED_BUILD (false)

// #define DETECT_MEMORY_OVERRUN
#define TRACK_MEMORY_BASIC    (0)
#define TRACK_MEMORY_VERBOSE  (1)
#define TRACK_MEMORY_DISABLED (2)

// If not defined, we will not track memory at all
// BASIC will track bytes used, and count
// VERBOSE will track individual callstacks
#if defined(_DEBUG)
#define TRACK_MEMORY          TRACK_MEMORY_DISABLED
#elif defined(FINAL_BUILD)
#define TRACK_MEMORY          TRACK_MEMORY_DISABLED
#else 
#define TRACK_MEMORY          TRACK_MEMORY_DISABLED
#endif
