#include "Engine/Net/NetSession.hpp"
#include "Engine/Net/NetConnection.hpp"
#include "Engine/Net/NetMessage.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

#include <algorithm>
#include <limits>
#include <type_traits>

//*******************************************************************
NetSession::NetSession()
    : m_connection(nullptr)
    , m_hostConnection(nullptr)
    , m_messageDefintions((std::numeric_limits<std::underlying_type_t<eMessageId>>::max)()
        , nullptr)
    , m_connections{}
    , m_connectionList{}
    , m_maxConnectionCount(16)
{}

//*******************************************************************
NetSession::~NetSession()
{
    if (m_connection != nullptr)
    {
        delete m_connection;
        m_connection = nullptr;
    }

    /*if (m_hostConnection != nullptr)
    {
        delete m_hostConnection;
        m_hostConnection = nullptr;
    }

    for (NetConnection* netConnection : m_connections)
        delete netConnection;
    m_connections.clear();

    for (NetMessageDefinition* netMessageDefinition : m_messageDefintions)
        delete netMessageDefinition;
    m_messageDefintions.clear();*/
}

//*******************************************************************
bool NetSession::RegisterMessageDefinition(uint8_t msg_id, NetMessageDefinition &defn)
{
    NetMessageDefinition* netMssgDfn = new NetMessageDefinition();
    netMssgDfn->m_typeIndex = defn.m_typeIndex;
    netMssgDfn->m_functionHandler = defn.m_functionHandler;
    m_messageDefintions[msg_id] = netMssgDfn;
    return true;
}

//*******************************************************************
NetMessageDefinition* NetSession::GetMessageDefinition(uint8_t id) const
{
    if (id >= m_messageDefintions.size())
        return nullptr;
    return m_messageDefintions[id];
}

//*******************************************************************
void NetSession::ProcessMessage(NetMessage* msg)
{

    if (msg == nullptr)
    {
        return;
    }
    
    NetMessageDefinition* netMsgDef = GetMessageDefinition(msg->m_messageTypeIndex);


    if (netMsgDef == nullptr)
    {
        msg = nullptr;
        return;
    }


    NetMessageDefinition* defn = GetMessageDefinition(msg->m_messageTypeIndex);


    if (defn)
    {
        defn->m_functionHandler(msg);
    }

}

//*******************************************************************
void NetSession::SetState(eSessionState state)
{
    m_state = state;
}

//*******************************************************************
uint8_t NetSession::GetFreeConnectionIndex() const
{
    uint index = 0;
    for (index = 0; index < m_connections.size(); ++index)
    {
        if (m_connections[index] == nullptr)
        {
            return (uint8_t)index;
        }
    }

    if (index < m_maxConnectionCount)
    {
        return (uint8_t)index;
    }
    else
    {
        return INVALID_CONNECTION_INDEX;
    }
}

//*******************************************************************
void NetSession::JoinConnection(uint8_t idx, NetConnection *conn)
{
    if (conn == nullptr)
        return;

    conn->m_connectionIndex = idx;

    if (idx >= m_connections.size())
    {
        m_connections.resize(idx + 1);
    }

    m_connections[idx] = conn;
    m_connectionList.push_back(conn);
}

//*******************************************************************
void NetSession::DestroyConnection(NetConnection *cp)
{
    if (nullptr == cp)
    {
        return;
    }

    if (m_connection == cp)
    {
        m_connection = nullptr;
    }

    if (m_hostConnection == cp)
    {
        m_hostConnection = nullptr;
    }

    if (cp->m_connectionIndex != INVALID_CONNECTION_INDEX)
    {
        m_connections[cp->m_connectionIndex] = nullptr;
        cp->m_connectionIndex = INVALID_CONNECTION_INDEX;
    }

    cp->m_owner = nullptr;
    
    auto foundConnection = std::find(m_connectionList.begin(), m_connectionList.end(), cp);
    if (foundConnection != m_connectionList.end())
    {
        m_connectionList.erase(std::remove(m_connectionList.begin(), m_connectionList.end(), cp), m_connectionList.end());
    }

    delete cp;
    cp = nullptr;
}

//*******************************************************************
NetConnection* NetSession::GetConnection(uint8_t idx)
{
    if (idx < m_connections.size())
    {
        return m_connections[idx];
    }
    return nullptr;
}

//*******************************************************************
void NetSession::SendMessageToOthers(NetMessage const &msg)
{
    for (int connectionListIndex = 0; connectionListIndex < (int)m_connectionList.size(); ++connectionListIndex)
    {
        NetConnection *cp = m_connectionList[connectionListIndex];
        if (cp != nullptr)
        {
            if (cp != m_connection)
            {
                NetMessage* netMsg = new NetMessage(msg);
                netMsg->m_sender = cp;
                cp->Send(netMsg);
            }
        }
    }
}
