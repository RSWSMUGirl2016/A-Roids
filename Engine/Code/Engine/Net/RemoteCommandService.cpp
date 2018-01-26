#include "Engine/Net/RemoteCommandService.hpp"
#include "Engine/Net/NetSession.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Net/NetConnection.hpp"

//*******************************************************************
RemoteCommandService* RemoteCommandService::s_instance = nullptr;

//*******************************************************************
RemoteCommandService::RemoteCommandService()
    : m_session(nullptr)
    , m_currentSender(nullptr)
{
    m_session = new TCPSession();
    s_instance = this;
    SetupMessageDefintions();
}

//*******************************************************************
RemoteCommandService::~RemoteCommandService()
{
    s_instance = nullptr;
}

//*******************************************************************
void RemoteCommandService::Update()
{
    if (m_session->IsRunning())
    {
        m_session->Update();
    }
    else
    {
        net_address_t addr = GetMyAddress(RCS_PORT);
        if (!m_session->Join(addr))
        {
            m_session->Host(RCS_PORT);
            m_session->StartListening();
        }
    }
}

//*******************************************************************
void RemoteCommandService::Join(net_address_t const &addr)
{
    m_session->Leave();
    m_session->Join(addr);
}

//*******************************************************************
void RemoteCommandService::SetupMessageDefintions()
{
    NetMessageDefinition *netMessDefOnCommand = new NetMessageDefinition();
    netMessDefOnCommand->m_typeIndex = SEND_COMMAND;
    netMessDefOnCommand->m_functionHandler = std::bind(&RemoteCommandService::OnCommand, this, std::placeholders::_1);
    m_session->RegisterMessageDefinition(SEND_COMMAND, *netMessDefOnCommand);

    NetMessageDefinition *netMessDefSendCommand = new NetMessageDefinition();
    netMessDefSendCommand->m_typeIndex = SEND_MESSAGE;
    netMessDefSendCommand->m_functionHandler = std::bind(&RemoteCommandService::OnMessage, this, std::placeholders::_1);
    m_session->RegisterMessageDefinition(SEND_MESSAGE, *netMessDefSendCommand);
}

//*******************************************************************
void RemoteCommandService::OnCommand(NetMessage *msg)
{
    std::string commandAndArgs = msg->ReadString();

    m_currentSender = msg->m_sender;
    g_theConsole->RunCommand(commandAndArgs);
    DebuggerPrintf(commandAndArgs.c_str());
    m_currentSender = nullptr;
}

//*******************************************************************
void RemoteCommandService::OnMessage(NetMessage *msg)
{
    std::string response = msg->ReadString();
    g_theConsole->ConsolePrintf(Rgba::WHITE, "[%s]", NetAddressToString(msg->m_sender->m_address));
    g_theConsole->ConsolePrintf(Rgba::WHITE, "%s", response);
}

//*******************************************************************
void RemoteCommandService::SendMessage(char const *str)
{
    NetMessage msg(SEND_MESSAGE);
    std::string msgStr(str);
    msg.WriteString(msgStr);

    m_currentSender->Send(&msg);
}

//*******************************************************************
void RemoteCommandService::SendCommandToOthers(char const *command_and_args)
{
    NetMessage message(SEND_COMMAND);
    message.WriteString(command_and_args);
    m_session->SendMessageToOthers(message);
}

