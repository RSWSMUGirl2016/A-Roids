#include "Engine/Net/Net.hpp"

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>

//winsock2 library
#pragma comment(lib, "ws2_32.lib")


bool NetSystemStartup()
{
    //pick the WinSock version
    WORD version = MAKEWORD(2, 2);

    WSADATA data;
    int error = ::WSAStartup(version, &data);
    return (error == 0);
}

void NetSystemShutdown()
{
    ::WSACleanup();
}
