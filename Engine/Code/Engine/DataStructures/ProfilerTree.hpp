#pragma once
#include <vector>

struct profiler_node_t
{
public:
    char const* m_tag;
    uint64_t m_startTime;
    uint64_t m_endTime;

    profiler_node_t* m_parent;
    std::vector<profiler_node_t*> m_children;
    profiler_node_t* m_previous;
    profiler_node_t* m_next;

    profiler_node_t()
        : m_tag(nullptr)
        , m_parent(nullptr)
        , m_previous(nullptr)
        , m_next(nullptr)
    {};

    void SetParent(profiler_node_t* parent);
};

class ProfilerTree
{
public:
    profiler_node_t* m_activeNode;
    profiler_node_t* m_root;
    std::vector<profiler_node_t*> m_completedFrames;
    bool m_pauseProfiler;

public:
    ProfilerTree() 
        : m_activeNode(nullptr)
        , m_root(nullptr)
        , m_pauseProfiler(false)
    {};
    ~ProfilerTree();

    void Push(char const* tag);
    bool Pop();
    profiler_node_t* GetPreviousFrame();
    profiler_node_t* GetPreviousFrame(char const *rootTag);
    void DeleteTree(profiler_node_t* head);
};