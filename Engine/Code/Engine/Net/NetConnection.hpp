#pragma once
#include "Engine/Net/netaddress.hpp"

class NetMessage;
class NetSession;

class NetConnection
{
public:
    virtual ~NetConnection();

    virtual void Send(NetMessage *msg) = 0;
    virtual bool Receive(NetMessage **msg) = 0;
    bool operator==(const NetConnection& netConnectionToEqual);
    bool operator!=(const NetConnection& netConnectionToNotEqual);

public:
    NetSession *m_owner;
    net_address_t m_address;
    uint8_t m_connectionIndex; // LUID 
};