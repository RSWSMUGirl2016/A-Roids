#pragma once
#include "Engine/Net/NetConnection.hpp"

#include <array>

class TCPSocket;

class TCPConnection : public NetConnection
{
public:
    virtual void Send(NetMessage *msg) override;
    virtual bool Receive(NetMessage **msg) override;
    bool Connect();
    bool IsDisconnected();

public:
    TCPSocket *m_socket;

    std::array<unsigned char, 1027> m_buffer = { 0 };
    unsigned int m_bufferBytesUsed = 0;

    int m_messageLength = 0;
    uint8_t m_messageType;
    byte_t m_messagePayload[1024];
    uint m_messagePayloadBytesUsed = 0;
};
