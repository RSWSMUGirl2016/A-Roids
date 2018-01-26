#include "Engine/Threading/Thread.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/MeshBuilder/FileBinaryStream.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Console.hpp"

#define PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>

//*******************************************************************
// Struct used to pass a name for the attached debugger
#define MS_VC_EXCEPTION      (0x406d1388)

#pragma pack(push, 8)
struct THREADNAME_INFO
{
    DWORD type;            // must be 0x1000
    const char *name;      // name
    DWORD thread_id;      // -1 for calling thread
    DWORD flags;         // must be 0, reserved for future use
};
#pragma pack(pop)

//*******************************************************************
struct thread_pass_data_t
{
    thread_cb cb;
    void *arg;
};

//*******************************************************************
static DWORD WINAPI ThreadEntryPointCommon(void *arg)
{
    thread_pass_data_t *pass_ptr = (thread_pass_data_t*)arg;

    pass_ptr->cb(pass_ptr->arg);
    delete pass_ptr;
    return 0;
}

//*******************************************************************
// Creates a thread with the entry point of cb, passed data
thread_handle_t ThreadCreate(thread_cb cb, void *data)
{
    // handle is like pointer, or reference to a thread
    // thread_id is unique identifier
    thread_pass_data_t *pass = new thread_pass_data_t();
    pass->cb = cb;
    pass->arg = data;

    DWORD thread_id;
    thread_handle_t th = (thread_handle_t) ::CreateThread(nullptr,   // SECURITY OPTIONS
        0,                         // STACK SIZE, 0 is default
        ThreadEntryPointCommon,    // "main" for this thread
        pass,                     // data to pass to it
        0,                         // initial flags
        &thread_id);              // thread_id

    return th;
}

//*******************************************************************
void ThreadSleep(uint ms)
{
    ::Sleep(ms);
}

//*******************************************************************
void ThreadYield()
{
    ::SwitchToThread();
}

//*******************************************************************
// Releases my hold on this thread.
void ThreadDetach(thread_handle_t th)
{
    ::CloseHandle(th);
}

//*******************************************************************
void ThreadJoin(thread_handle_t th)
{
    ::WaitForSingleObject(th, INFINITE);
    ::CloseHandle(th);
}


//*******************************************************************
thread_id_t ThreadGetCurrentID()
{
    return (thread_id_t)(uintptr_t) ::GetCurrentThreadId();
}

//*******************************************************************
void ThreadSetNameInVisualStudio(char const *name)
{
    if (nullptr == name) {
        return;
    }

    thread_id_t id = ThreadGetCurrentID();
    if (0 != id) {
        THREADNAME_INFO info;
        info.type = 0x1000;
        info.name = name;
        info.thread_id = (DWORD)(uintptr_t)id;
        info.flags = 0;

        __try
        {
            RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)(&info));
        }
        __except (EXCEPTION_CONTINUE_EXECUTION)
        {
        }
    }
}

////*******************************************************************
//void GenerateGarbageWork(void *data)
//{
//    UNUSED(data);
//    for (uint i = 0; i < 1000; ++i) {
//        printf("Garbage Thread: %i\n", i);
//    }
//}

//*******************************************************************
//void GenerateGarbage(char const *filename, size_t byte_count)
//{
//    UNUSED(filename);
//    UNUSED(byte_count);
//    thread_handle_t th = ThreadCreate(GenerateGarbageWork, nullptr);
//    ThreadJoin(th);
//}

//*******************************************************************
#include "Engine/Threading/CriticalSection.hpp"
#include "Engine/DataStructures/ThreadSafeQueue.hpp"

ThreadSafeQueue<std::string> gMessages;

thread_handle_t gLoggerThread = nullptr;
bool gLoggerThreadRunning = true;
bool gFlushMessageRequest = true;
Signal gLogSignal;
Event<std::string*> gLogEvent;

FileBinaryStream fileBinaryStream = FileBinaryStream();
std::vector<thread_handle_t> gLoggerThreads;
std::vector<char const*> gDisabledTags;

//*******************************************************************
Signal* GetLogSignal()
{
    return &gLogSignal;
}

//*******************************************************************
Event<std::string*>* GetLogEvent()
{
    return &gLogEvent;
}

//*******************************************************************
bool IsLoggerThreadRunning()
{
    return gLoggerThreadRunning;
}

//*******************************************************************
uint FlushMessages(FILE *fh)
{
    uint count = 0;
    std::string msg;

    while (gMessages.pop(&msg)) {
        gLogEvent.trigger(&msg);
        ++count;
    }

    if (gFlushMessageRequest)
    {
        fflush(fh);
        gFlushMessageRequest = false;
    }

    return count;
}

//*******************************************************************
void LogFlush()
{
    gFlushMessageRequest = true;

    while (gFlushMessageRequest)
    {
        // Loops until Flush has finished pushing to file
        ThreadYield();
    }
}

//*******************************************************************
void LogWriteToFile(void *user_arg, std::string *msg)
{
    FILE *fh = (FILE*)user_arg;
    fprintf(fh, "%s", msg->c_str());
}

//*******************************************************************
void LogWriteToDebugger(void *user_arg, std::string *msg)
{
    UNUSED(user_arg);
    OutputDebugStringA(msg->c_str());
    OutputDebugStringA("\n");
}

//*******************************************************************
void LoggerThread(void* user_arg)
{
    FILE *fh = nullptr;

    char const* path = (char const*)user_arg;
    if (path == nullptr)
        path = "Data/Log/log.log";

    errno_t err = fopen_s(&fh, path, "w+");
    if ((err != 0) || (fh == nullptr)) {
        return;
    }

    gLogEvent.subscribe(fh, LogWriteToFile);

    while (gLoggerThreadRunning) {
        gLogSignal.wait();
        FlushMessages(fh);
    }

    FlushMessages(fh);
    fclose(fh);
}

//*******************************************************************
void LogPrint(char const *msg)
{
    gMessages.push(msg);
    gLogSignal.signal_all();
}

//*******************************************************************
void LogPrintf(char const *format, ...)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    va_list variableArgumentList;
    va_start(variableArgumentList, format);
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArgumentList);
    va_end(variableArgumentList);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    gMessages.push(messageLiteral);
    gLogSignal.signal_all();
}

//*******************************************************************
void LogStartup()
{
    gLoggerThreadRunning = true;
    gLoggerThread = ThreadCreate(LoggerThread, nullptr);
}

//*******************************************************************
void LogStartup(char const *path)
{
    gLoggerThreadRunning = true;
    gLoggerThread = ThreadCreate(LoggerThread, (void*)path);
}

//*******************************************************************
void LogShutdown()
{
    gLoggerThreadRunning = false;
    gLogSignal.signal_all();
    ThreadJoin(gLoggerThread);
    gLoggerThread = INVALID_THREAD_HANDLE;

}

//*******************************************************************
void ThreadDemo()
{
    // GenerateGarbage( "garbage.dat", 50 * 1024 * 1024 );
    LogStartup();

    gLogEvent.subscribe(nullptr, LogWriteToDebugger);

    for (uint i = 0; i < 1000; ++i) {
        LogPrint("Main Thread!");
        ThreadSleep(10);
    }

    LogShutdown();
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
void LogTest(int thread_count, int line_count)
{
    gLoggerThreadRunning = true;
    for (int thread_count_index = 0; thread_count_index < thread_count; thread_count_index++)
    {
        gLoggerThreads.push_back(ThreadCreate(LoggerThread, "Data/Log/LogTest.log"));
    }

    for (int line_count_index = 1; line_count_index <= line_count; line_count_index++)
    {
        for (int thread_count_index = 1; thread_count_index <= thread_count; thread_count_index++)
        {
            LogPrintf("Thread[%i], writing line[%i]\n", thread_count_index, line_count_index);
            ThreadSleep(10);
        }
    }

    gLoggerThreadRunning = false;
    for (int thread_count_index = 0; thread_count_index < thread_count; thread_count_index++)
    {        
        gLogSignal.signal_all();
        ThreadDetach(gLoggerThreads[thread_count_index]);        
    }
    gLoggerThreads.clear();
    gLoggerThread = INVALID_THREAD_HANDLE;
}

//*******************************************************************
//*******************************************************************
//*******************************************************************

ThreadSafeQueue<std::string> gTestMessages;

thread_handle_t gLoggerFlushThread = nullptr;
bool gLoggerFlushThreadRunning = true;
bool gFlushTestMessageRequest = true;

//*******************************************************************
void LogPrintFlushTestf(char const *format, ...)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    va_list variableArgumentList;
    va_start(variableArgumentList, format);
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArgumentList);
    va_end(variableArgumentList);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    gTestMessages.push(messageLiteral);
    gLogSignal.signal_all();
}

//*******************************************************************
uint FlushTestMessages(FILE *fh)
{
    uint count = 0;
    std::string msg;

    while (gTestMessages.pop(&msg)) {
        gLogEvent.trigger(&msg);
        ++count;
    }

    if (gFlushTestMessageRequest)
    {
        fflush(fh);
        gFlushTestMessageRequest = false;
    }

    return count;
}


//*******************************************************************
void LogTestFlush()
{
    gFlushTestMessageRequest = true;

    while (gFlushTestMessageRequest)
    {
        // Loops until Flush has finished pushing to file
        ThreadYield();
    }
}

//*******************************************************************
void LoggerFlushThreadTest(void* user_arg)
{
    FILE *fh = nullptr;

    char const* path = (char const*)user_arg;
    if (path == nullptr)
        path = "Data/Log/log.log";

    errno_t err = fopen_s(&fh, path, "w+");
    if ((err != 0) || (fh == nullptr)) {
        return;
    }

    gLogEvent.subscribe(fh, LogWriteToFile);    
    FlushTestMessages(fh);

    fclose(fh);
}

//*******************************************************************
void LogFlushTest(char const *text)
{
    gLoggerFlushThreadRunning = true;
    const char* path = "Data/Log/LogFlushTest.log";
    gLoggerFlushThread = ThreadCreate(LoggerFlushThreadTest, (void*)path);

    LogPrintFlushTestf(text);
    ThreadSleep(10);

    LogTestFlush();

    gLoggerFlushThreadRunning = false;
    gLogSignal.signal_all();
    ThreadDetach(gLoggerFlushThread);
    gLoggerFlushThread = INVALID_THREAD_HANDLE;

    __debugbreak();
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
struct thread_pass_two_data_t
{
    thread_cb_two cb;
    void *arg1;
    void *arg2;
};

//*******************************************************************
static DWORD WINAPI ThreadEntryPointCommonTwo(void *arg)
{
    thread_pass_two_data_t *pass_ptr = (thread_pass_two_data_t*)arg;

    pass_ptr->cb(pass_ptr->arg1, pass_ptr->arg2);
    delete pass_ptr;
    return 0;
}

//*******************************************************************
thread_handle_t ThreadCreateTwo(thread_cb_two cb, void *data1, void *data2)
{
    // handle is like pointer, or reference to a thread
    // thread_id is unique identifier
    thread_pass_two_data_t *pass = new thread_pass_two_data_t();
    pass->cb = cb;
    pass->arg1 = data1;
    pass->arg2 = data2;

    DWORD thread_id;
    (thread_handle_t) ::CreateThread(nullptr,   // SECURITY OPTIONS
        0,                         // STACK SIZE, 0 is default
        ThreadEntryPointCommonTwo,    // "main" for this thread
        pass,                     // data to pass to it
        0,                         // initial flags
        &thread_id);              // thread_id

    return nullptr;
}

//*******************************************************************
uint FlushMessagesTwo(FILE *fh)
{
    UNUSED(fh);

    uint count = 0;
    std::string msg;

    while (gMessages.pop(&msg)) {
        //gLogEvent.triggerTwo(&msg);
        ++count;
    }

    return count;
}

//*******************************************************************
void LogWriteBytesToFile(void *user_arg1, void *user_arg2, void *event_arg)
{
    UNUSED(user_arg1);
    UNUSED(user_arg2);
    //FILE *fh = (FILE*)user_arg2;
    //std::string *name = (std::string*)user_arg1;
    std::string *msg = (std::string*)event_arg;

    //fprintf(fh, "%s\n", msg->c_str());

    std::string msg_string = *msg;
    fileBinaryStream.write_bytes(msg_string.c_str(), msg_string.length());
}

//*******************************************************************
void LoggerThreadTest(void* data1, void* data2)
{
    UNUSED(data2);

    FILE *fh = nullptr;
    errno_t err = fopen_s(&fh, (const char*)data1, "wb+");
    if ((err != 0) || (fh == nullptr)) {
        return;
    }
    fileBinaryStream.file_pointer = fh;

    //gLogEvent.subscribeTwo(data1, fh, LogWriteBytesToFile);

    while (gLoggerThreadRunning) {
        gLogSignal.wait();
        FlushMessagesTwo(fh);
    }

    FlushMessagesTwo(fh);
    fclose(fh);
}

//*******************************************************************
void LogStartupTest(char const *path)
{
    gLoggerThreadRunning = true;
    gLoggerThread = ThreadCreateTwo(LoggerThreadTest, (void*)path, nullptr);
}

//*******************************************************************
void LogShutdownTest()
{
    gLoggerThreadRunning = false;
    gLogSignal.signal_all();
    ThreadJoin(gLoggerThread);
    gLoggerThread = INVALID_THREAD_HANDLE;

}

//************************************************************************
void ThreadTest(char const *path, uint byte_size)
{
    LogStartupTest(path);

    //gLogEvent.subscribeTwo((void*)path, nullptr, LogWriteBytesToFile);

    for (uint i = 0; i < byte_size; ++i) {
        LogPrintf("%i", 1);
        ThreadSleep(10);
    }

    LogShutdownTest();
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
void LogPrintfv(char const *format, va_list variableArguments)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArguments);
    va_end(variableArguments);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    gMessages.push(messageLiteral);
    gLogSignal.signal_all();
}

//*******************************************************************
void LogWarningf(char const *format, va_list variableArguments)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArguments);
    va_end(variableArguments);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    gMessages.push(messageLiteral);
    gLogSignal.signal_all();
}

//*******************************************************************
void LogErrorf(char const *format, va_list variableArguments)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, variableArguments);
    va_end(variableArguments);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

    gMessages.push(messageLiteral);
    gLogSignal.signal_all();

    LogFlush();
    __debugbreak();
}

//*******************************************************************
bool IsTagDisabled(char const *tag)
{
    typedef std::vector<const char*> CharStarContainer;
    typedef CharStarContainer::iterator CharStarIterator;

    CharStarIterator tagResult = find(gDisabledTags.begin(), gDisabledTags.end(), tag);

    if (tagResult != gDisabledTags.end()) {
        //found it
        return true;
    }
    else
    {
        //does not exist
        return false;
    }
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
#include "Engine/Core/Time.hpp"
void OnLogPrintToDevConsole(char const* tag, char const *format, va_list varargs)
{
    if (strcmp(tag, "profile") != 0)
    {
        g_theConsole->AddLogMessage(tag, format, varargs);
    }    
}

//*******************************************************************
void OnLogPrintToDebugger(char const *format, va_list varargs)
{
    const int MESSAGE_MAX_LENGTH = 2048;
    char messageLiteral[MESSAGE_MAX_LENGTH];
    vsnprintf_s(messageLiteral, MESSAGE_MAX_LENGTH, _TRUNCATE, format, varargs);
    va_end(varargs);
    messageLiteral[MESSAGE_MAX_LENGTH - 1] = '\0'; // In case vsnprintf overran (doesn't auto-terminate)

#if defined( PLATFORM_WINDOWS )
    if (IsDebuggerAvailable())
    {
        OutputDebugStringA(messageLiteral);
    }
#endif

    std::cout << messageLiteral;
}

//*******************************************************************
void LogTaggedPrintf(char const *tag, char const *format, ...)
{
    bool isTagInDisabledList = IsTagDisabled(tag);
    if (isTagInDisabledList)
        return;

    std::string tagStr = "";
    std::string timeStamp = GetCurrentDateAndTime();
    va_list variableArgumentList;

    if (strcmp(tag, "default") == 0)
    {
        g_theConsole->DevConsoleSetTagColor(tag, Rgba::WHITE);

        tagStr = "[default] [";
        tagStr.append(timeStamp);
        tagStr.append("] ");
        tagStr.append(format);

        va_start(variableArgumentList, format);
        LogPrintfv(tagStr.c_str(), variableArgumentList);    
    }
    else if (strcmp(tag, "warning") == 0)
    {
        g_theConsole->DevConsoleSetTagColor(tag, Rgba(255, 255, 0, 255));

        tagStr = "[warning] [";
        tagStr.append(timeStamp);
        tagStr.append("] ");
        tagStr.append(format);

        va_start(variableArgumentList, format);
        LogWarningf(tagStr.c_str(), variableArgumentList);
    }
    else if (strcmp(tag, "error") == 0)
    {
        g_theConsole->DevConsoleSetTagColor(tag, Rgba(255, 0, 0, 255));

        tagStr = "[error] [";
        tagStr.append(timeStamp);
        tagStr.append("] ");
        tagStr.append(format);

        va_start(variableArgumentList, format);
        LogErrorf(tagStr.c_str(), variableArgumentList);
    }
    else if (strcmp(tag, "profile") == 0)
    {
        g_theConsole->DevConsoleSetTagColor(tag, Rgba(255, 0, 0, 255));

        tagStr = "[profile] ";
        tagStr.append(format);

        va_start(variableArgumentList, format);
        LogPrintfv(tagStr.c_str(), variableArgumentList);
    }


    va_start(variableArgumentList, format);
    OnLogPrintToDevConsole(tag, tagStr.c_str(), variableArgumentList);
    va_start(variableArgumentList, format);
    OnLogPrintToDebugger(format, variableArgumentList);
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
void LogDisable(char const *tag)
{
    gDisabledTags.push_back(tag);
}

//*******************************************************************
void LogEnable(char const *tag)
{
    gDisabledTags.erase(std::remove(gDisabledTags.begin(), gDisabledTags.end(), tag), gDisabledTags.end());
}

//*******************************************************************
void LogDisableAll()
{
    gDisabledTags.push_back("error");
    gDisabledTags.push_back("warning");
    gDisabledTags.push_back("default");
}

//*******************************************************************
void LogEnableAll()
{
    gDisabledTags.clear();
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
#include <ctime>
void CopyAndRenameLogFile(char *fileDirectories, char* logName)
{
    time_t currentTime = time(0);
    tm* currentDate = localtime(&currentTime);
    char filename[256] = { 0 };

    strcat(filename, Stringf("%d%d%d_%d%d%d", currentDate->tm_mday, currentDate->tm_mon + 1, currentDate->tm_year + 1900, currentDate->tm_hour, currentDate->tm_min, currentDate->tm_sec).c_str());

    char path[256];
    strcpy(path, fileDirectories);
    strcat(path, logName);
    strcat(path, ".log");

    char newPath[256];
    strcpy(newPath, fileDirectories);
    strcat(newPath, logName);
    strcat(newPath, "_");
    strcat(newPath, filename);
    strcat(newPath, ".log");

    WCHAR fromPath[1024];
    MultiByteToWideChar(GetACP(), 0, path, -1, fromPath, sizeof(fromPath) / sizeof(fromPath[0]));
    WCHAR toPath[1024];
    MultiByteToWideChar(GetACP(), 0, newPath, -1, toPath, sizeof(toPath) / sizeof(toPath[0]));

    BOOL b = CopyFile(fromPath, toPath, TRUE);
    if (!b) {
        std::cout << "Error: " << GetLastError();
    }
    else
    {
        std::cout << "Okay ";
    }
}
