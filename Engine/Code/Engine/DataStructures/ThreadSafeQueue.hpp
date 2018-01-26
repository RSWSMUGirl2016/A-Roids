#pragma once
#include "Engine/EngineCommon.hpp"
#include "Engine/Threading/CriticalSection.hpp"

#include <queue>
// Infoknowledge Management System

//*******************************************************************
template <typename T>
class ThreadSafeQueue
{
public:
    std::queue<T> m_queue;
    CriticalSection m_lock;

public:
    //*******************************************************************
    ~ThreadSafeQueue()
    {
        int a = 10;
        a = a;
    };

    //*******************************************************************
    void push(T const &v)
    {
        SCOPE_LOCK(m_lock);
        m_queue.push(v);
    };

    //*******************************************************************
    bool empty()
    {
        SCOPE_LOCK(m_lock);
        bool result = m_queue.empty();

        return result;
    };

    //*******************************************************************
    bool pop(T *out)
    {
        SCOPE_LOCK(m_lock);

        if (m_queue.empty()) {
            return false;
        }
        else {
            *out = m_queue.front();
            m_queue.pop();
            return true;
        }
    };

    //*******************************************************************
    T front()
    {
        SCOPE_LOCK(m_lock);
        T result = m_queue.front();

        return result;
    };
};