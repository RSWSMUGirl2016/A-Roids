#include "Engine/Net/LoopBackConnection.hpp"
#include "Engine/Net/NetMessage.hpp"

//*******************************************************************
LoopBackConnection::~LoopBackConnection()
{}

//*******************************************************************
void LoopBackConnection::Send(NetMessage *msg)
{
    m_messageQueue.push(msg);
}

//*******************************************************************
bool LoopBackConnection::Receive(NetMessage **msg)
{
    if (m_messageQueue.empty())
        return false;
    *msg = m_messageQueue.front();
    m_messageQueue.pop();
    return true;
}
