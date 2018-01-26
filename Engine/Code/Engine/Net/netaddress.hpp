#pragma once
#include "Engine/EngineCommon.hpp"

#include <stdint.h>
#include <inttypes.h>
#include <vector>
#include <string>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <typeinfo>

struct net_address_t
{
    // host name
    // port
    uint address;
    uint16_t port;
};


typedef unsigned short  u_short;

struct sockaddr;

bool NetAddressFromSocketAddress(net_address_t *out, sockaddr *socket_address);
void SocketAddressFromNetAddress(sockaddr *out, int *out_size, net_address_t const &addr);
std::vector<net_address_t> GetAddressesFromHostName(char const *hostname, uint16_t port, bool bindable);
std::string NetAddressToString(net_address_t &addr);
net_address_t StringToNetAddress(const char *addr);
net_address_t GetMyAddress(uint16_t port);