#pragma once
#include "Engine/Net/NetSession.hpp"


const uint16_t RCS_PORT = 44444;


class TCPSocket;


class TCPSession : public NetSession
{
public:
    TCPSession();
    virtual ~TCPSession();

public:
    // create a loopback - add it to connections, set my and host connection to it;
    virtual bool Host(uint16_t port) override;
    // unimplemented so far;
    virtual bool Join(net_address_t const &addr) override; // production version: session_join_info_t instead
                                                           // if I have a connection - destroy it.
    virtual void Leave() override;
    // Process all connections; 
    virtual void Update() override;

    void SendJoinInfo(NetConnection *cp);
    void OnJoinResponse(NetMessage *msg);
    bool StartListening();
    void StopListening();
    bool IsListening();

public:
    TCPSocket *m_listenSocket;
    std::function<void(NetMessage*)> m_handlerFunc;
    NetMessage* m_previousNetMessage;
    uint8_t m_previousNetMessageTypeID = INVALID_CONNECTION_INDEX;
    uint16_t m_previousNetMessageNetID = 0;
};