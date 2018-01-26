#pragma once
#include "Engine/Net/TCPSession.hpp"

class RemoteCommandService
{
public:
    RemoteCommandService();
    ~RemoteCommandService();

public:
    void Update();
    void Join(net_address_t const &addr);
    void SetupMessageDefintions();
    void OnCommand(NetMessage *msg);
    void OnMessage(NetMessage *msg);
    void SendMessage(char const *str);
    void SendCommandToOthers(char const *command_and_args);

public:
    TCPSession *m_session;
    NetConnection *m_currentSender;

public:
    static RemoteCommandService* GetInstance() { return s_instance; }

private:
    static RemoteCommandService *s_instance;
}; 