#include "Engine/Net/tcpsocket.hpp"

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

//winsock2 library
#pragma comment(lib, "ws2_32.lib")

TCPSocket::TCPSocket()
    : m_socket(INVALID_SOCKET)
    , m_isListenSocket(false)
{};

TCPSocket::~TCPSocket()
{
    Close();
};

bool TCPSocket::IsValid() const
{
    return (m_socket != INVALID_SOCKET);
}

void TCPSocket::Close()
{
    if (IsValid())
    {
        ::closesocket(m_socket);
        m_socket = INVALID_SOCKET;
        m_isListenSocket = false;
    }
}

bool TCPSocket::Join(const char* addr)
{
    net_address_t net_addr = StringToNetAddress(addr);
    return Join(net_addr);
}

bool TCPSocket::Join(net_address_t addr)
{
    if (IsValid())
    {
        return false;
    }

    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_storage internal_address;
    int addr_size = 0;
    SocketAddressFromNetAddress((sockaddr*)&internal_address, &addr_size, addr);

    int result = ::connect(sock, (sockaddr*)&internal_address, addr_size);


    if (result != 0)
    {
        int error = WSAGetLastError();
        DebuggerPrintf("TCPSocket::join failed, Error: %u\n", error);
        ::closesocket(sock);
        return false;
    }
    else
    {
        m_socket = sock;
        m_address = addr;
        return true;
    }
}

int TCPSocket::Send(void const *payload, uint payload_size_in_bytes)
{
    if (!IsValid())
    {
        return 0;
    }

    if (m_isListenSocket)
    {
        //do a recoverable error
        return 0;
    }

    if (payload_size_in_bytes == 0)
    {
        return 0;
    }

    int bytes_sent = ::send(m_socket, (char const *)payload, (int)payload_size_in_bytes, 0);
    if (bytes_sent <= 0)
    {
        int error = WSAGetLastError();
        DebuggerPrintf("TCPSocket::send failed, Error: %u\n", error);
        Close();
        return 0;
    }

    ASSERT_OR_DIE((uint)bytes_sent == payload_size_in_bytes, "bytes cannot be the same size as payload size");
    return bytes_sent;
}

bool TCPSocket::CheckForDisconnect()
{
    if (!IsValid())
    {
        return true;
    }

    WSAPOLLFD fd;
    fd.fd = m_socket;
    fd.events = POLLRDNORM;

    if (SOCKET_ERROR == WSAPoll(&fd, 1, 0))
    {
        return true;
    }

    if ((fd.revents & POLLHUP) != 0)
    {
        Close();
        return true;
    }

    return false;
}

bool TCPSocket::IsDisconnected()
{
    if (!IsValid())
    {
        return true;
    }

    WSAPOLLFD fd;
    fd.fd = m_socket;
    fd.events = POLLRDNORM;

    if (SOCKET_ERROR == WSAPoll(&fd, 1, 0))
    {
        return true;
    }

    if ((fd.revents & POLLHUP) != 0)
    {
        Close();
        return false;
    }
    return true;
}

int TCPSocket::Receive(void *payload, uint max_payload_size)
{
    if (!IsValid() || (max_payload_size == 0))
    {
        return 0;
    }

    if (m_isListenSocket)
    {
        //do a recoverable error
        return 0;
    }

    ASSERT_OR_DIE(payload != nullptr, "Payload cannot be a nullptr.");

    int bytes_read = ::recv(m_socket, (char*)payload, (int)max_payload_size, 0);
    if (bytes_read <= 0)
    {
        if (bytes_read == SOCKET_ERROR)
        {
            int error = WSAGetLastError();
            if (error == WSAEWOULDBLOCK)
            {
                return 0;
            }
            else
            {
                Close();
                return 0;
            }
        }
        else
        {
            if (CheckForDisconnect())
            {
                return 0;
            }
        }
        return 0;
    }
    else
    {
        return (uint)bytes_read;
    }
}

void TCPSocket::SetBlocking(bool blocking)
{
    if (!IsValid())
    {
        return;
    }

    u_long non_blocking = blocking ? 0 : 1;
    ::ioctlsocket(m_socket, FIONBIO, &non_blocking);
}

bool TCPSocket::Listen(uint16_t port)
{
    if (IsValid())
    {
        return false;
    }

    std::vector<net_address_t> addresses = GetAddressesFromHostName("", port, true);
    if (addresses.empty())
    {
        return false;
    }

    SOCKET listen_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listen_socket == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_storage listen_address;
    uint addr_size = 0;
    SocketAddressFromNetAddress((sockaddr*)&listen_address, (int*)&addr_size, addresses[0]);

    int result = ::bind(listen_socket, (sockaddr*)&listen_address, (int)addr_size);
    if (result == SOCKET_ERROR)
    {
        ::closesocket(listen_socket);
        return false;
    }

    int max_queued = 8;
    result = ::listen(listen_socket, (int)max_queued);
    if (result == SOCKET_ERROR)
    {
        ::closesocket(listen_socket);
        return false;
    }

    m_socket = listen_socket;
    m_address = addresses[0];
    m_isListenSocket = true; // bool member of TCPSocket - defaults to false.
    return true;
}

bool TCPSocket::IsListening() const
{
    return m_isListenSocket;
}

TCPSocket* TCPSocket::Accept()
{
    if (!IsListening())
    {
        return nullptr;
    }

    sockaddr_storage conn_addr;
    int conn_addr_len = sizeof(conn_addr);

    SOCKET their_socket = ::accept(m_socket, (sockaddr*)&conn_addr, &conn_addr_len);
    if (their_socket == INVALID_SOCKET)
    {
        return nullptr;
    }

    net_address_t net_addr;
    if (!NetAddressFromSocketAddress(&net_addr, (sockaddr*)&conn_addr))
    {
        ::closesocket(their_socket);
        return nullptr;
    }

    TCPSocket *their_tcp_socket = new TCPSocket();
    their_tcp_socket->m_socket = their_socket;
    their_tcp_socket->m_address = net_addr;

    return their_tcp_socket;
}
