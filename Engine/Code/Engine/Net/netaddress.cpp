#include "Engine/Net/netaddress.hpp"

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

//winsock2 library
#pragma comment(lib, "ws2_32.lib")

bool NetAddressFromSocketAddress(net_address_t *out, sockaddr *socket_address)
{
    if (nullptr == socket_address)
    {
        return false;
    }

    if (socket_address->sa_family != AF_INET)
    {
        return false;
    }

    sockaddr_in* ipv4_address = (sockaddr_in*)socket_address;

    out->port = ntohs(ipv4_address->sin_port);
    out->address = ntohl(ipv4_address->sin_addr.S_un.S_addr);

    return true;
}

void SocketAddressFromNetAddress(sockaddr *out, int *out_size, net_address_t const &addr)
{
    sockaddr_in *out_addr = (sockaddr_in*)out;
    memset(out_addr, 0, sizeof(sockaddr_in));
    out_addr->sin_family = AF_INET;
    out_addr->sin_port = htons(addr.port);
    out_addr->sin_addr.S_un.S_addr = htonl(addr.address);

    *out_size = sizeof(sockaddr_in);
}

std::vector<net_address_t> GetAddressesFromHostName(char const *hostname, uint16_t port, bool bindable)
{
    char service[16];
    sprintf_s(service, "%u", port);

    addrinfo hints;
    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // udp = SOCK_DGRAM

    hints.ai_flags |= (bindable ? AI_PASSIVE : 0);
    //hints.ai_flags != AI_NUMERICHOST; // do not want this if passing strings like google.com, if you are only passing numeric addresses, this is fine

    addrinfo *results = nullptr;
    int status = ::getaddrinfo(hostname, service, &hints, &results);

    std::vector<net_address_t> addresses;
    //ASSERT HERE AS WELL
    if (status != 0)
    {
        return addresses;
    }

    addrinfo *addr = results;
    while (nullptr != addr)
    {
        net_address_t net_addr;
        if (NetAddressFromSocketAddress(&net_addr, addr->ai_addr))
        {
            addresses.push_back(net_addr);
        }
        addr = addr->ai_next;
    }

    ::freeaddrinfo(results);
    return addresses;
}

std::string NetAddressToString(net_address_t &addr)
{
    unsigned char *arr = (unsigned char *)&addr.address;
    unsigned char a = arr[0];
    unsigned char b = arr[1];
    unsigned char c = arr[2];
    unsigned char d = arr[3];

    std::string string_addr = std::to_string(d) + "." + std::to_string(c) + "." + std::to_string(b) + "." + std::to_string(a) + ":" + std::to_string(addr.port);
    return string_addr;
}

net_address_t StringToNetAddress(const char *addr)
{
    std::istringstream iStringStream(addr);
    std::string stringToken;

    net_address_t net_addr;
    int currentIndex = 0;
    std::string address = "";

    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;
    std::string::size_type sz;

    while (std::getline(iStringStream, stringToken, '.'))
    {
        int num = std::stoi(stringToken, &sz);

        if (currentIndex == 0)
            a = (int)num;
        if (currentIndex == 1)
            b = (int)num;
        if (currentIndex == 2)
            c = (int)num;

        if (currentIndex == 3)
        {
            std::istringstream iColonStringStream(stringToken);
            int colonIndex = 0;
            while (std::getline(iColonStringStream, stringToken, ':'))
            {
                num = std::stoi(stringToken, &sz);
                if (colonIndex == 0)
                {
                    d = (int)num;
                }
                else
                {
                    char *end;
                    intmax_t val = strtoimax(stringToken.c_str(), &end, 10);
                    net_addr.port = (uint16_t)val;
                }
                colonIndex++;
            }
        }

        currentIndex++;
    }

    int temp_addr = 0;
    a = a << 24;
    b = b << 16;
    c = c << 8;
    d = d << 0;

    temp_addr = a + b + c + d;

    net_addr.address = (unsigned int)temp_addr;

    return net_addr;
}

net_address_t GetMyAddress(uint16_t port)
{
    std::vector<net_address_t> addresses = GetAddressesFromHostName("", port, true);
    return addresses[0];
}
