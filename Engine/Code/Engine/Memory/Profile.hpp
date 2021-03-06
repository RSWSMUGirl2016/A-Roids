#pragma once

/************************************************************************/
/*                                                                      */
/* INCLUDE                                                              */
/*                                                                      */
/************************************************************************/
#include "Engine/EngineCommon.hpp"
#include "Engine/Core/Time.hpp"

/************************************************************************/
/*                                                                      */
/* DEFINES AND CONSTANTS                                                */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* MACROS                                                               */
/*                                                                      */
/************************************************************************/
#define PROFILE_LOG_SCOPE(s) ProfileLogScope __pscope_##__LINE__##(s)

/************************************************************************/
/*                                                                      */
/* TYPES                                                                */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* STRUCTS                                                              */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* CLASSES                                                              */
/*                                                                      */
/************************************************************************/
class ProfileLogScope
{
public:
    ProfileLogScope(char const *n)
    {
        name = n;
        start_op = TimeGetOpCount();
    }

    ~ProfileLogScope()
    {
        uint64_t end_op = TimeGetOpCount();
        uint64_t elapsed = end_op - start_op;

        printf("%s [%llu ops, %s]\n", name, elapsed, TimeOpCountToString(elapsed));
    }

public:
    char const *name;
    uint64_t start_op;
};

/************************************************************************/
/*                                                                      */
/* GLOBAL VARIABLES                                                     */
/*                                                                      */
/************************************************************************/

/************************************************************************/
/*                                                                      */
/* FUNCTION PROTOTYPES                                                  */
/*                                                                      */
/************************************************************************/
