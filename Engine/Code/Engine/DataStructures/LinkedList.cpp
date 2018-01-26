#include "Engine/DataStructures/LinkedList.hpp"

//*******************************************************************
void LinkedList::CreateNode(Callstack *callstack, uint allocation)
{
    Node *temp = (Node*)malloc(sizeof(Node));
    temp->m_callstack = callstack;
    temp->m_allocation = allocation;
    temp->m_next = nullptr;
    if (m_head == nullptr)
    {
        m_head = temp;
        m_tail = temp;
        temp = nullptr;
    }
    else
    {
        m_tail->m_next = temp;
        m_tail = temp;
    }
}

//*******************************************************************
void LinkedList::InsertStart(Callstack *callstack, uint allocation)
{
    Node *temp = (Node*)malloc(sizeof(Node));
    temp->m_callstack = callstack;
    temp->m_allocation = allocation;
    temp->m_next = m_head;
    m_head = temp;
}

//*******************************************************************
void LinkedList::InsertPosition(int position, Callstack *callstack, uint allocation)
{
    Node *previous = (Node*)malloc(sizeof(Node));
    Node *current = (Node*)malloc(sizeof(Node));
    Node *temp = (Node*)malloc(sizeof(Node));
    current = m_head;

    for (int index = 1; index < position; index++)
    {
        previous = current;
        current = current->m_next;
    }

    temp->m_callstack = callstack;
    temp->m_allocation = allocation;
    previous->m_next = temp;
    temp->m_next = current;
}

//*******************************************************************
void LinkedList::DeleteFirst()
{
    Node *temp = (Node*)malloc(sizeof(Node));
    temp = m_head;
    m_head = m_head->m_next;
    DestroyCallstack(temp->m_callstack);
    free(temp);    
    //delete temp;
    //temp = nullptr;
}

//*******************************************************************
void LinkedList::DeleteLast()
{
    if (m_head == nullptr)
        return;

    Node *current = (Node*)malloc(sizeof(Node));
    Node *previous = (Node*)malloc(sizeof(Node));
    current = m_head;
    
    while (current->m_next != nullptr)
    {
        previous = current;
        current = current->m_next;
    }

    m_tail = previous;
    previous->m_next = nullptr;
    DestroyCallstack(current->m_callstack);
    free(current);
    //delete current;
    //current = nullptr;
}

//*******************************************************************
void LinkedList::DeletePosition(int position)
{
    Node *current = (Node*)malloc(sizeof(Node));
    Node *previous = (Node*)malloc(sizeof(Node));
    current = m_head;

    for (int index = 1; index < position; index++)
    {
        previous = current;
        current = current->m_next;
    }

    previous->m_next = current->m_next;
}
