#pragma once
#include "Engine/Net/netaddress.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

typedef UINT_PTR SOCKET;

class TCPSocket
{
public:
    SOCKET m_socket;
    bool m_isListenSocket;
    net_address_t m_address;

    TCPSocket();
    ~TCPSocket();

public:
    bool IsValid() const;
    void Close();

    // CLIENT
    bool Join(const char* addr);
    bool Join(net_address_t addr);

    // BOTH
    int Send(void const *payload, uint payload_size_in_bytes);
    bool CheckForDisconnect();
    bool IsDisconnected();
    int Receive(void *payload, uint max_payload_size);
    void SetBlocking(bool blocking);

    // HOST
    bool Listen(uint16_t port);
    bool IsListening() const;
    TCPSocket* Accept();
};