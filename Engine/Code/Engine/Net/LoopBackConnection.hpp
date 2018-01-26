#pragma once
#include "Engine/Net/NetConnection.hpp"
#include <queue>

class LoopBackConnection : public NetConnection
{
public:
    virtual ~LoopBackConnection();

    virtual void Send(NetMessage *msg) override;		// enqueue
    virtual bool Receive(NetMessage **msg) override;	// dequeue

public:
    std::queue<NetMessage*> m_messageQueue;
};