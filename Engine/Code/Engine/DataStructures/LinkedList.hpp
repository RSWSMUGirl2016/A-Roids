#pragma once
#include "Engine/Memory/Callstack.hpp"
#include "Engine/EngineCommon.hpp"

struct Node
{
public:
    Callstack *m_callstack;
    uint m_allocation;
    Node *m_next;

    Node()
    {
        m_callstack = nullptr;
        m_allocation = 0;
        m_next = nullptr;
    }
};

class LinkedList
{
public:
    Node *m_head;
    Node *m_tail;

public:
    LinkedList()
    {
        m_head = nullptr;
        m_tail = nullptr;
    }

    void CreateNode(Callstack *data, uint allocation);
    void InsertStart(Callstack *data, uint allocation);
    void InsertPosition(int position, Callstack *callstack, uint allocation);
    void DeleteFirst();
    void DeleteLast();
    void DeletePosition(int position);
};
