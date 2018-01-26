#include "Engine/Profiler/Profiler.hpp"
#include "Engine/EngineCommon.hpp"
#include "Engine/Threading/Thread.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <algorithm>

//*******************************************************************
Profiler* g_profiler = nullptr;

//*******************************************************************
void Profiler::ProfilerSystemStartup()
{
    m_threadID = ::GetCurrentThreadId();
}

//*******************************************************************
void Profiler::ProfilerSystemShutdown()
{}

//*******************************************************************
void Profiler::ProfilerPush(char const* tag)
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerPush() is not running on the Main Thread");

    if (m_profilerTree.m_activeNode == nullptr)
        m_profilerTree = ProfilerTree();


    m_profilerTree.Push(tag);
    m_numberOfPushes++;
}

//*******************************************************************
void Profiler::ProfilerPop()
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerPush() is not running on the Main Thread");

    bool killTree = m_profilerTree.Pop();
    m_numberOfPops++;

    if (killTree)
        m_completedFrames.push_back(m_profilerTree);
}

//*******************************************************************
profiler_node_t* Profiler::ProfilerGetPreviousFrame()
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerPush() is not running on the Main Thread");

    return m_profilerTree.GetPreviousFrame();
}

//*******************************************************************
profiler_node_t* Profiler::ProfilerGetPreviousFrame(char const *rootTag)
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerPush() is not running on the Main Thread");

    return m_profilerTree.GetPreviousFrame(rootTag);
}

//*******************************************************************
void Profiler::ProfilerPause()
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerPush() is not running on the Main Thread");

    m_profilerTree.m_pauseProfiler = true;
}

//*******************************************************************
void Profiler::ProfilerResume()
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerPush() is not running on the Main Thread");

    m_profilerTree.m_pauseProfiler = false;
}

//*******************************************************************
ProfilerTree* Profiler::ProfilerGetPreviousFrameTree()
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerGetPreviousFrameTree() is not running on the Main Thread");

    if (!m_completedFrames.empty())
    {
        return &m_completedFrames.back();
    }
    return nullptr;
}

//*******************************************************************
profiler_node_t* Profiler::ProfilerGetPreviousFrameTree(char const *rootTag)
{
    if (m_threadID != ::GetCurrentThreadId())
        ASSERT_OR_DIE(false, "Profiler.cpp, ProfilerGetPreviousFrameTree() is not running on the Main Thread");

    if (!m_completedFrames.empty())
    {
        return m_completedFrames.back().GetPreviousFrame(rootTag);
    }
    return nullptr;
}

//*******************************************************************
//*******************************************************************
//*******************************************************************
ProfilerReport::~ProfilerReport()
{}

//*******************************************************************
void ProfilerReport::CreateTreeView()
{
    if (m_reportedFrame == nullptr)
        ASSERT_OR_DIE(false, "There is no frame to make a report from, ProfilerReport::CreateTreeView(), Profiler.cpp, line 80");

    TraverseTree(m_reportedFrame->m_root, nullptr);
}

//*******************************************************************
void ProfilerReport::CreateFlatView()
{
    if (m_reportedFrame == nullptr)
        ASSERT_OR_DIE(false, "There is no frame to make a report from, ProfilerReport::CreateTreeView(), Profiler.cpp, line 80");

    TraverseFlat(m_reportedFrame->m_root);
}

//*******************************************************************
void ProfilerReport::TraverseTree(profiler_node_t* node, profiler_node_t* prevNode)
{
    if (node != nullptr)
    {        

        if (prevNode != nullptr)
        {
            if (strcmp(node->m_tag, prevNode->m_tag) == 0)
            {
                report_node_t* reportNode = GetTreeReportNode(node);
                reportNode->m_numberOfCalls++;

                uint64_t newNodesTotalTime = node->m_endTime - node->m_startTime;
                reportNode->m_totalTime = reportNode->m_totalTime + newNodesTotalTime;

                uint64_t newNodesSelfTime = newNodesTotalTime - GetTotalOfChildrensTime(node);
                reportNode->m_selfTime = reportNode->m_selfTime + newNodesSelfTime;
            }         
            else
            {
                report_node_t* reportNode = new report_node_t();
                reportNode->m_tag = node->m_tag;
                SetParentTag(reportNode, node);
                reportNode->m_numberOfCalls = 1;
                reportNode->m_totalTime = node->m_endTime - node->m_startTime;
                reportNode->m_selfTime = reportNode->m_totalTime - GetTotalOfChildrensTime(node);
                m_reportTreeNodes.push_back(reportNode);
            }
        }
        else
        {
            report_node_t* reportNode = new report_node_t();
            reportNode->m_tag = node->m_tag;
            SetParentTag(reportNode, node);
            reportNode->m_numberOfCalls = 1;
            reportNode->m_totalTime = node->m_endTime - node->m_startTime;
            reportNode->m_selfTime = reportNode->m_totalTime - GetTotalOfChildrensTime(node);
            m_reportTreeNodes.push_back(reportNode);
        }        

        for (int nodeIndex = 0; nodeIndex < (int)node->m_children.size(); nodeIndex++)
        {
            TraverseTree(node->m_children[nodeIndex], prevNode);
            prevNode = node->m_children[nodeIndex];
        }
        prevNode = node;
    }

}

//*******************************************************************
void ProfilerReport::TraverseFlat(profiler_node_t* node)
{
    if (node != nullptr)
    {
        bool isThereAlreadyAReportNode = DoesThereExistAReportNode(node);

        if (isThereAlreadyAReportNode)
        {
            report_node_t* reportNode = GetFlatReportNode(node);
            reportNode->m_numberOfCalls++;

            uint64_t newNodesTotalTime = node->m_endTime - node->m_startTime;
            reportNode->m_totalTime = reportNode->m_totalTime + newNodesTotalTime;

            uint64_t newNodesSelfTime = newNodesTotalTime - GetTotalOfChildrensTime(node);
            reportNode->m_selfTime = reportNode->m_selfTime + newNodesSelfTime;
        }
        else
        {
            report_node_t* reportNode = new report_node_t();
            reportNode->m_tag = node->m_tag;
            SetParentTag(reportNode, node);
            reportNode->m_numberOfCalls = 1;
            reportNode->m_totalTime = node->m_endTime - node->m_startTime;
            reportNode->m_selfTime = reportNode->m_totalTime - GetTotalOfChildrensTime(node);
            m_reportFlatNodes.push_back(reportNode);
        }

        for (int nodeIndex = 0; nodeIndex < (int)node->m_children.size(); nodeIndex++)
        {
            TraverseFlat(node->m_children[nodeIndex]);
        }
    }

}

//*******************************************************************
void ProfilerReport::SetParentTag(report_node_t* reportNode, profiler_node_t* profilerNode)
{
    if (profilerNode->m_parent == nullptr)
    {
        reportNode->m_parentTag = "";
    }
    else
    {
        reportNode->m_parentTag = profilerNode->m_parent->m_tag;
    }
}

//*******************************************************************
bool ProfilerReport::DoesThereExistAReportNodeWithSameParentExist(profiler_node_t* node)
{
    const char* parentTag = "";
    if (node->m_parent != nullptr)
    {
        parentTag = node->m_parent->m_tag;
    }

    for (int reportNodeIndex = 0; reportNodeIndex < (int)m_reportTreeNodes.size(); reportNodeIndex++)
    {
        if ((strcmp(node->m_tag, m_reportTreeNodes[reportNodeIndex]->m_tag) == 0) && (strcmp(parentTag, m_reportTreeNodes[reportNodeIndex]->m_parentTag) == 0))
        {
            return true;
        }
    }

    return false;
}

//*******************************************************************
bool ProfilerReport::DoesThereExistAReportNode(profiler_node_t* node)
{
    for (int reportNodeIndex = 0; reportNodeIndex < (int)m_reportFlatNodes.size(); reportNodeIndex++)
    {
        if (strcmp(node->m_tag, m_reportFlatNodes[reportNodeIndex]->m_tag) == 0)
        {
            return true;
        }
    }

    return false;
}

//*******************************************************************
report_node_t* ProfilerReport::GetTreeReportNode(profiler_node_t* node)
{
    const char* parentTag = "";
    if (node->m_parent != nullptr)
    {
        parentTag = node->m_parent->m_tag;
    }

    for (int reportNodeIndex = (int)m_reportTreeNodes.size() - 1; reportNodeIndex >= 0; reportNodeIndex--)
    {
        if ((strcmp(node->m_tag, m_reportTreeNodes[reportNodeIndex]->m_tag) == 0) && (strcmp(parentTag, m_reportTreeNodes[reportNodeIndex]->m_parentTag) == 0))
        {
            return m_reportTreeNodes[reportNodeIndex];
        }
    }

    return nullptr;
}

//*******************************************************************
report_node_t* ProfilerReport::GetFlatReportNode(profiler_node_t* node)
{
   for (int reportNodeIndex = 0; reportNodeIndex < (int)m_reportFlatNodes.size(); reportNodeIndex++)
    {
        if (strcmp(node->m_tag, m_reportFlatNodes[reportNodeIndex]->m_tag) == 0)
        {
            return m_reportFlatNodes[reportNodeIndex];
        }
    }

    return nullptr;
}

//*******************************************************************
uint64_t ProfilerReport::GetTotalOfChildrensTime(profiler_node_t* parentNode)
{
    uint64_t childrensTime = 0;
    for (int childIndex = 0; childIndex < (int)parentNode->m_children.size(); childIndex++)
    {
        childrensTime = childrensTime + (parentNode->m_children[childIndex]->m_endTime - parentNode->m_children[childIndex]->m_startTime);
    }
    return childrensTime;
}

//*******************************************************************
void ProfilerReport::LogTreeView()
{
    uint64_t totalTime = m_reportedFrame->m_root->m_endTime - m_reportedFrame->m_root->m_startTime;
    LogTaggedPrintf("profile", "%s:          %s          %s          %s          %s          %s \n", "TAG NAME", "CALLS", "TOTAL%", "TOTAL TIME", "SELF%", "SELF TIME");
    for (int treeReportNodeIndex = 0; treeReportNodeIndex < (int)m_reportTreeNodes.size(); treeReportNodeIndex++)
    {
        report_node_t* node = m_reportTreeNodes[treeReportNodeIndex];
        LogTaggedPrintf("profile", "%s:          %i          %f          %s          %f           %s \n", node->m_tag, node->m_numberOfCalls, (float)(node->m_totalTime / totalTime) * 100.f, TimeOpCountToString(node->m_totalTime), (float)(node->m_selfTime / totalTime) * 100.f, TimeOpCountToString(node->m_selfTime) );
    }    
}

//*******************************************************************
void ProfilerReport::LogFlatView()
{
    uint64_t totalTime = m_reportedFrame->m_root->m_endTime - m_reportedFrame->m_root->m_startTime;
    LogTaggedPrintf("profile", "%s:          %s          %s          %s          %s          %s \n", "TAG NAME", "CALLS", "TOTAL%", "TOTAL TIME", "SELF%", "SELF TIME");
    for (int flatReportNodeIndex = 0; flatReportNodeIndex < (int)m_reportFlatNodes.size(); flatReportNodeIndex++)
    {
        report_node_t* node = m_reportFlatNodes[flatReportNodeIndex];
        LogTaggedPrintf("profile", "%s:          %i          %f          %s          %f           %s \n", node->m_tag, node->m_numberOfCalls, (float)(node->m_totalTime / totalTime) * 100.f, TimeOpCountToString(node->m_totalTime), (float)(node->m_selfTime / totalTime) * 100.f, TimeOpCountToString(node->m_selfTime));
    }
}


//*******************************************************************
struct greaterTotalTime
{
    bool operator()(report_node_t* const &a, report_node_t* const &b) const { return a->m_totalTime > b->m_totalTime; }
};

//*******************************************************************
struct greaterSelfTime
{
    bool operator()(report_node_t* const &a, report_node_t* const &b) const { return a->m_selfTime > b->m_selfTime; }
};

//*******************************************************************
void ProfilerReport::SortTreeByTotalTime()
{
    std::sort(m_reportTreeNodes.begin(), m_reportTreeNodes.end(), greaterTotalTime());
}

//*******************************************************************
void ProfilerReport::SortTreeBySelfTime()
{
    std::sort(m_reportTreeNodes.begin(), m_reportTreeNodes.end(), greaterSelfTime());
}

//*******************************************************************
void ProfilerReport::SortFlatByTotalTime()
{
    std::sort(m_reportFlatNodes.begin(), m_reportFlatNodes.end(), greaterTotalTime());
}

//*******************************************************************
void ProfilerReport::SortFlatBySelfTime()
{
    std::sort(m_reportFlatNodes.begin(), m_reportFlatNodes.end(), greaterSelfTime());
}