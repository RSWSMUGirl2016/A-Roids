#include "Engine/DataStructures/ProfilerTree.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
void profiler_node_t::SetParent(profiler_node_t* parent)
{
    if (parent == nullptr)
    {
        m_parent = parent;
        return;
    }
        
    m_parent = parent;
    parent->m_children.push_back(this);
}

//*******************************************************************
ProfilerTree::~ProfilerTree()
{
    if (m_activeNode != nullptr)
    {
        delete m_activeNode;
        m_activeNode = nullptr;
    }

    DeleteTree(m_root);

    for (int completedFramesIndex = 0; completedFramesIndex < (int)m_completedFrames.size(); completedFramesIndex++)
    {
        if (m_completedFrames[completedFramesIndex] != nullptr)
        {
            //delete m_completedFrames[completedFramesIndex];
            m_completedFrames[completedFramesIndex] = nullptr;
        }
    }
    m_completedFrames.clear();

}

//*******************************************************************
void ProfilerTree::Push(char const* tag)
{
    profiler_node_t* node = new profiler_node_t();
    node->m_tag = tag;
    node->m_startTime = GetCurrentPerformanceCounter();
    node->SetParent(m_activeNode);
    m_activeNode = node;

    if (m_root == nullptr)
        m_root = node;
}

//*******************************************************************
bool ProfilerTree::Pop()
{
    if (m_activeNode == nullptr)
        ASSERT_OR_DIE(false, "Profiler: Active Node is nullptr");

    m_activeNode->m_endTime = GetCurrentPerformanceCounter();    

    if (m_activeNode->m_parent == nullptr)
    {       
        if (m_pauseProfiler)
            return false;

        m_completedFrames.push_back(m_activeNode);
        m_activeNode = nullptr;
        
        return true;
    }

    m_activeNode = m_activeNode->m_parent;
    return false;
}

//*******************************************************************
profiler_node_t* ProfilerTree::GetPreviousFrame()
{
    if (!m_completedFrames.empty()) 
    {
        return m_completedFrames.back();
    }
    return nullptr;
}

//*******************************************************************
profiler_node_t* ProfilerTree::GetPreviousFrame(char const *rootTag)
{
    if (!m_completedFrames.empty()) 
    {
        for (int completedFramesIndex = (int)m_completedFrames.size() - 1; completedFramesIndex >= 0; completedFramesIndex--)
        {
            if (strcmp(m_completedFrames[completedFramesIndex]->m_tag, rootTag) == 0)
            {
                return m_completedFrames[completedFramesIndex];
            }
        }
    }
    return nullptr;
}

//*******************************************************************
void ProfilerTree::DeleteTree(profiler_node_t* head)
{
    if (head != nullptr)
    {
        for (int nodeIndex = 0; nodeIndex < (int)head->m_children.size(); nodeIndex++)
        {
            DeleteTree(head->m_children[nodeIndex]);
        }
        delete head;
        head = nullptr;
    }
}

