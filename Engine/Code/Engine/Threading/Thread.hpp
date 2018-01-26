#pragma once
#include "Engine/EngineCommon.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// Used for parameter forwarding
#include <tuple>
#include <utility>

#include <string>

#define INVALID_THREAD_HANDLE 0

// empty structs to make C++ tell me if I'm casting wrong.
struct thread_handle_name_t {};

// typedefs, since I'm dealing only with pointers
typedef thread_handle_name_t* thread_handle_t;
typedef uint thread_id_t;

// Infoknowledge Management System
typedef void(*thread_cb)(void*);
typedef void(*thread_cb_two)(void*, void*);

// Used for parameter forwarding
template <typename CB, typename ...ARGS>
struct pass_data_t
{
    CB cb;
    std::tuple<ARGS...> args;

    pass_data_t(CB cb, ARGS ...args)
        : cb(cb)
        , args(args...) {}
};


// Creates a thread with the entry point of cb, passed data
thread_handle_t ThreadCreate(thread_cb cb, void *data);
thread_handle_t ThreadCreateTwo(thread_cb_two cb, void* data1, void *data2);

void ThreadSleep(uint ms);

void ThreadYield();
// Releases my hold on this thread [one of these MUST be called per create]
void ThreadDetach(thread_handle_t th);
void ThreadJoin(thread_handle_t th);

void LogPrint(char const *msg);
void LogPrintf(char const *format, ...);
void LogStartup(char const *path);
void LogStartup();
void LogShutdown();
void ThreadDemo();

void LogTest(int thread_count, int line_count);
void ThreadTest(char const *path, uint byte_size);
void LogPrintfv(char const *format, va_list variableArguments);

#include "Engine/Threading/Signal.hpp"
#include "Engine/Threading/Event.hpp"

//*******************************************************************
Signal* GetLogSignal();
Event<std::string*>* GetLogEvent();
bool IsLoggerThreadRunning();
//*******************************************************************

uint FlushMessages(FILE *fh);
void LogFlush();
void LogWriteToFile(void *user_arg, std::string *msg);
void LogFlushTest(char const *text);

void LogTaggedPrintf(char const *tag, char const *format, ...);
void LogDisable(char const *tag);
void LogEnable(char const *tag);
void LogDisableAll();
void LogEnableAll();
void CopyAndRenameLogFile(char *fileDirectories, char* logName);

// Get the current thread id
thread_id_t ThreadGetCurrentID();

void ThreadSetNameInVisualStudio(char const *name);

//------------------------------------------------------------------------
// Templated Versions;
//------------------------------------------------------------------------

//------------------------------------------------------------------------
template <typename CB, typename TUPLE, size_t ...INDICES>
void ForwardArgumentsWithIndices(CB cb, TUPLE &args, std::integer_sequence<size_t, INDICES...>)
{
    cb(std::get<INDICES>(args)...);
}

//------------------------------------------------------------------------
template <typename CB, typename ...ARGS>
void ForwardArgumentsThread(void *ptr)
{
    pass_data_t<CB, ARGS...> *args = (pass_data_t<CB, ARGS...>*) ptr;
    ForwardArgumentsWithIndices(args->cb, args->args, std::make_index_sequence<sizeof...(ARGS)>());
    delete args;
}

//------------------------------------------------------------------------
template <typename CB, typename ...ARGS>
thread_handle_t ThreadCreateWithArguments(CB entry_point, ARGS ...args)
{
    pass_data_t<CB, ARGS...> *pass = new pass_data_t<CB, ARGS...>(entry_point, args...);
    return ThreadCreate(ForwardArgumentsThread<CB, ARGS...>, (void*)pass);
}
