#pragma once

#define PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//*******************************************************************
#include "Engine/Core/Time.hpp"
#include "Engine/DataStructures/ProfilerTree.hpp"
#include <vector>

class Profiler
{
public:
    ProfilerTree m_profilerTree;
    std::vector<ProfilerTree> m_completedFrames;
    int m_numberOfPushes;
    int m_numberOfPops;
    DWORD m_threadID;

    Profiler()
        : m_profilerTree(ProfilerTree())
        , m_numberOfPushes(0)
        , m_numberOfPops(0)
    {};
    ~Profiler()
    {};

    void ProfilerSystemStartup();
    void ProfilerSystemShutdown();
    void ProfilerPush(char const* tag);
    void ProfilerPop();
    profiler_node_t* ProfilerGetPreviousFrame();
    profiler_node_t* ProfilerGetPreviousFrame(char const *rootTag);
    void ProfilerPause();
    void ProfilerResume();
    ProfilerTree* ProfilerGetPreviousFrameTree();
    profiler_node_t* ProfilerGetPreviousFrameTree(char const *rootTag);
};

extern Profiler* g_profiler;

//*******************************************************************
#define PROFILE_LOG_SCOPE(s) ProfileLogScope __pscope(s)

//*******************************************************************
class ProfileLogScope
{
public:
    ProfileLogScope(char const *n)
    {
        name = n;
        start_op = TimeGetOpCount();

        g_profiler->ProfilerPush(name);
    }

    ~ProfileLogScope()
    {
        uint64_t end_op = TimeGetOpCount();
        uint64_t elapsed = end_op - start_op;
        
        g_profiler->ProfilerPop();

        printf("%s [%llu ops, %s]\n", name, elapsed, TimeOpCountToString(elapsed));
    }

public:
    char const *name;
    uint64_t start_op;
};

//*******************************************************************
#include <map>

struct report_node_t
{
public:
    char const* m_tag;
    char const* m_parentTag;
    int m_numberOfCalls;
    uint64_t m_totalTime;
    uint64_t m_selfTime;

    report_node_t()
        : m_tag("")
        , m_numberOfCalls(0)
        , m_totalTime(0)
        , m_selfTime(0)
    {}
};

class ProfilerReport
{
public:
    ProfilerTree* m_reportedFrame;
    uint64_t m_totalTime;
    std::vector<report_node_t*> m_reportTreeNodes;
    std::vector<report_node_t*> m_reportFlatNodes;
    
    ProfilerReport()
        : m_reportedFrame(nullptr)
    {};
    ProfilerReport(ProfilerTree* reportedFrame)
        : m_reportedFrame(reportedFrame)
    {
        if (m_reportedFrame == nullptr)
        {
            return;
        }
        m_totalTime = m_reportedFrame->m_root->m_endTime - m_reportedFrame->m_root->m_startTime;
    };
    ~ProfilerReport();

    void CreateTreeView();
    void CreateFlatView();
    void TraverseTree(profiler_node_t* node, profiler_node_t* prevNode);
    void TraverseFlat(profiler_node_t* node);
    void SetParentTag(report_node_t* reportNode, profiler_node_t* profilerNode);
    bool DoesThereExistAReportNodeWithSameParentExist(profiler_node_t* node);
    bool DoesThereExistAReportNode(profiler_node_t* node);
    report_node_t* GetTreeReportNode(profiler_node_t* node);
    report_node_t* GetFlatReportNode(profiler_node_t* node);
    uint64_t GetTotalOfChildrensTime(profiler_node_t* parentNode);
    void LogTreeView();
    void LogFlatView();
    void SortTreeByTotalTime();
    void SortTreeBySelfTime();
    void SortFlatByTotalTime();
    void SortFlatBySelfTime();
};