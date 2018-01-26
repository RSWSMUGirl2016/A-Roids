#include "Engine/Net/NetConnection.hpp"

//*******************************************************************
NetConnection::~NetConnection()
{}

//*******************************************************************
bool NetConnection::operator==(const NetConnection& netConnectionToEqual)
{
    if (m_address.address != netConnectionToEqual.m_address.address)
        return false;
    if (m_address.port != netConnectionToEqual.m_address.port)
        return false;
    if (m_connectionIndex != netConnectionToEqual.m_connectionIndex)
        return false;
    return true;
}

//*******************************************************************
bool NetConnection::operator!=(const NetConnection& netConnectionToNotEqual)
{
    return !(*(this) == netConnectionToNotEqual);
}

