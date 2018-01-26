#pragma once
#include "Engine/Net/NetMessageDefinition.hpp"
#include "Engine/Net/netaddress.hpp"
#include <vector>

class NetConnection;
class NetMessage;

enum eSessionState : uint8_t
{
    SESSION_DISCONNECTED = 0,
    SESSION_CONNECTING = 1,
    SESSION_CONNECTED = 2,
};

constexpr uint8_t INVALID_CONNECTION_INDEX = 0xff;

class NetSession
{
public:
    NetSession();
    virtual ~NetSession();

public:
    virtual bool Host(uint16_t port) = 0;
    virtual bool Join(net_address_t const &addr) = 0; // production version: session_join_info_t instead
    virtual void Leave() = 0;
    virtual void Update() = 0;

    virtual bool StartListening() = 0;
    virtual void StopListening() = 0;
    virtual bool IsListening() = 0;

public:
    bool RegisterMessageDefinition(uint8_t msg_id, NetMessageDefinition &defn);
    NetMessageDefinition* GetMessageDefinition(uint8_t id) const;

    void ProcessMessage(NetMessage* msg);

    void SetState(eSessionState state);


    inline bool AmIHost() const
    {
        return (m_connection == m_hostConnection) && (m_hostConnection != nullptr);
    };

    inline bool AmIClient() const
    {
        return (m_connection != m_hostConnection) && (m_connection != nullptr);
    };

    inline bool IsRunning() const
    {
        return (m_connection != nullptr);
    };

    inline bool IsReady() const
    {
        return (m_state == SESSION_CONNECTED);
    };

    uint8_t GetFreeConnectionIndex() const;
    void JoinConnection(uint8_t idx, NetConnection *conn);
    void DestroyConnection(NetConnection *cp);
    NetConnection* GetConnection(uint8_t idx);
    void SendMessageToOthers(NetMessage const &msg);

public:
    eSessionState m_state;

    std::vector<NetConnection*> m_connectionList;

    // connection info
    std::vector<NetConnection*> m_connections;
    uint m_maxConnectionCount;

    NetConnection *m_connection;		// helpers
    NetConnection *m_hostConnection; 	// 

                                        // message data;
    std::vector<NetMessageDefinition*> m_messageDefintions;

};
