#include "Engine/Net/TCPSession.hpp"
#include "Engine/Net/NetConnection.hpp"
#include "Engine/Net/LoopBackConnection.hpp"
#include "Engine/Net/tcpsocket.hpp"
#include "Engine/Net/NetMessage.hpp"
#include "Engine/Net/TCPConnection.hpp"
#include "Engine/EngineCommon.hpp"

//*******************************************************************
TCPSession::TCPSession()
    : m_listenSocket(nullptr)
    , m_previousNetMessage(nullptr)
{
    NetMessageDefinition *netMessageDefinition = new NetMessageDefinition();
    netMessageDefinition->m_typeIndex = (eMessageId)JOIN_RESPONSE;
    netMessageDefinition->m_functionHandler = std::bind(&TCPSession::OnJoinResponse, this, std::placeholders::_1);

    RegisterMessageDefinition(JOIN_RESPONSE, *netMessageDefinition);
}

//*******************************************************************
TCPSession::~TCPSession()
{
    if (m_listenSocket != nullptr)
    {
        delete m_listenSocket;
        m_listenSocket = nullptr;
    }
}

//*******************************************************************
bool TCPSession::Host(uint16_t port)
{
    UNUSED(port);
    ASSERT_OR_DIE(!IsRunning(), "TCPSession::Host, server is not running.");

    m_connection = new LoopBackConnection();
    m_connection->m_address = GetMyAddress(port);
    m_connection->m_owner = this;

    JoinConnection(0, m_connection);
    m_hostConnection = m_connection;

    SetState(SESSION_CONNECTED);

    return true;
}

//*******************************************************************
bool TCPSession::Join(net_address_t const &addr)
{
    TCPConnection *host = new TCPConnection();
    host->m_address = addr;
    host->m_owner = this;
    host->m_socket = new TCPSocket();


    // Try to connect to host
    if (!host->Connect())
    {
        Leave();
        host = nullptr;
        return false;
    }

    host->m_socket->SetBlocking(false);

    JoinConnection(0, host); // 0 for this class;
    m_hostConnection = host;

    m_connection = new LoopBackConnection();
    m_connection->m_address = GetMyAddress(addr.port);
    m_connection->m_owner = this;

    SetState(SESSION_CONNECTING);
    return true;
}

//*******************************************************************
void TCPSession::Leave()
{
    DestroyConnection(m_connection);
    DestroyConnection(m_hostConnection);

    for (uint index = 0; index < (uint)m_connections.size(); ++index)
    {
        DestroyConnection(m_connections[index]);
    }
    m_connections.clear();

    StopListening();
    SetState(SESSION_DISCONNECTED);
}

//*******************************************************************
void TCPSession::Update()
{
    if (IsListening())
    {
        TCPSocket *socket = m_listenSocket->Accept();
        if (socket != nullptr)
        {
            socket->SetBlocking(false);

            TCPConnection *newGuy = new TCPConnection();
            newGuy->m_address = socket->m_address;
            newGuy->m_socket = socket;
            newGuy->m_owner = this;
            newGuy->m_socket->SetBlocking(false);

            uint8_t connIdx = GetFreeConnectionIndex();
            if (connIdx == INVALID_CONNECTION_INDEX)
            {
                delete newGuy;
            }
            else
            {
                newGuy->m_connectionIndex = connIdx;
                newGuy->m_owner->m_state = SESSION_CONNECTED;
                JoinConnection(newGuy->m_connectionIndex, newGuy);
                SendJoinInfo(newGuy);
            }
        }
    }


    for (uint index = 0; index < (uint)m_connectionList.size(); ++index)
    {


        NetConnection* netConnection = m_connectionList[index];
        if (netConnection == nullptr)
        {
            continue;
        }

        NetMessage* netMessage = nullptr;

        bool isMessageReceiving = netConnection->Receive(&netMessage);

        //make receive return a bool!!!!
        while (netMessage)
        {       
            if (netMessage->m_messageTypeIndex == 45)
            {
                std::string netMsgStr = netMessage->ReadString();
                std::string delimiter = ",";

                std::string netMsgToken = netMsgStr.substr(0, netMsgStr.find(delimiter));
                uint16_t netMsgNetID = (uint16_t)std::stoi(netMsgToken);

                if (m_previousNetMessageNetID != netMsgNetID)
                {
                    m_previousNetMessageNetID = netMsgNetID;

                    netMessage->m_sender = netConnection;
                    ProcessMessage(netMessage);
                    delete netMessage;
                    netMessage = nullptr;
                    isMessageReceiving = netConnection->Receive(&netMessage);
                }
                else
                {
                    netMessage->m_sender = netConnection;
                    ProcessMessage(netMessage);
                    delete netMessage;
                    netMessage = nullptr;
                    isMessageReceiving = netConnection->Receive(&netMessage);
                }
            }
            else
            {
                netMessage->m_sender = netConnection;
                ProcessMessage(netMessage);
                delete netMessage;
                netMessage = nullptr;
                isMessageReceiving = netConnection->Receive(&netMessage);
            }

            

            /*if (netMessage)
            {
                if (netMessage->m_messageTypeIndex == 45)
                {
                    std::string netMsgStr = netMessage->ReadString();
                    std::string delimiter = ",";

                    std::string netMsgToken = netMsgStr.substr(0, netMsgStr.find(delimiter));
                    uint16_t netMsgNetID = (uint16_t)std::stoi(netMsgToken);

                    if (m_previousNetMessageNetID != netMsgNetID)
                    {
                        m_previousNetMessageNetID = netMsgNetID;

                        netMessage->m_sender = netConnection;
                        ProcessMessage(netMessage);
                        delete netMessage;
                        netMessage = nullptr;
                        isMessageReceiving = netConnection->Receive(&netMessage);
                    }
                }
            }    */  

        }
    }


    for (uint index = 0; index < (uint)m_connections.size(); ++index)
    {
        NetConnection *cp = m_connections[index];
        if ((cp != nullptr) && (cp != m_connection))
        {
            TCPConnection *tcpConnection = (TCPConnection*)cp;
            if (tcpConnection->IsDisconnected())
            {
                DestroyConnection(tcpConnection);
                tcpConnection = nullptr;
            }
        }
    }


    if (m_hostConnection == nullptr)
    {
        Leave();
    }
}

//*******************************************************************
void TCPSession::SendJoinInfo(NetConnection *cp)
{
    NetMessage *msg = new NetMessage(JOIN_RESPONSE);
    msg->m_payloadBytesUsed += msg->write_byte((byte_t)cp->m_connectionIndex);
    cp->Send(msg);
}

//*******************************************************************
void TCPSession::OnJoinResponse(NetMessage *msg)
{
    uint8_t connIndex;
    msg->read_byte(&connIndex);

    JoinConnection(connIndex, m_connection);
    SetState(SESSION_CONNECTED);
}

//*******************************************************************
bool TCPSession::StartListening()
{
    if (!AmIHost())
    {
        return false;
    }

    if (IsListening())
    {
        return true;
    }

    m_listenSocket = new TCPSocket();

    if (m_listenSocket->Listen(m_connection->m_address.port))
    {
        m_listenSocket->SetBlocking(false);
        return true;
    }
    else
    {
        delete m_listenSocket;
        m_listenSocket = nullptr;
        return false;
    }
}

//*******************************************************************
void TCPSession::StopListening()
{
    if (IsListening())
    {
        delete m_listenSocket;
        m_listenSocket = nullptr;
    }
}

//*******************************************************************
bool TCPSession::IsListening()
{
    return (nullptr != m_listenSocket);
}

