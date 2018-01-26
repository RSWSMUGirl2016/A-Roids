#include "Engine/Net/TCPConnection.hpp"
#include "Engine/Net/NetMessage.hpp"
#include "Engine/Net/tcpsocket.hpp"

//*******************************************************************
void TCPConnection::Send(NetMessage *msg)
{
    unsigned short packet = (unsigned short)(msg->m_payloadBytesUsed + 1);

    m_socket->Send(&packet, sizeof(packet));
    m_socket->Send(&msg->m_messageTypeIndex, sizeof(msg->m_messageTypeIndex));
    m_socket->Send(msg->m_payload, msg->m_payloadBytesUsed);
}

//*******************************************************************
bool TCPConnection::Receive(NetMessage **msg)
{

    bool m_returnValue = true;


    if (m_bufferBytesUsed < 2)
    {

        unsigned int recvCount = m_socket->Receive(m_buffer.data() + m_bufferBytesUsed, 2 - m_bufferBytesUsed);
        if (recvCount == 0)
        {
            return m_returnValue;
            //return;
        }

        m_bufferBytesUsed += recvCount;
    }
    if(m_bufferBytesUsed >= 2)
    {

        uint16_t bytesToRead = *(uint16_t*)m_buffer.data();
        if (m_bufferBytesUsed < (2u + bytesToRead))
        {

            unsigned int recvCount = m_socket->Receive(m_buffer.data() + m_bufferBytesUsed, (2u + bytesToRead) - m_bufferBytesUsed);
            if (recvCount == 0)
            {
                return m_returnValue;
                //return;
            }
            m_bufferBytesUsed += recvCount;

            if (m_bufferBytesUsed >= (2u + bytesToRead))
            {

                *msg = new NetMessage(m_buffer.data()[2]);
                
                std::vector<unsigned char> buffer;
                buffer.resize(bytesToRead - 1);
                std::memcpy(buffer.data(), m_buffer.data() + 3, bytesToRead - 1);
                (*msg)->write_bytes(buffer.data(), buffer.size());                

                m_bufferBytesUsed = 0;
                m_returnValue = false;

            }
        }

    }

    return m_returnValue;
}

//*******************************************************************
bool TCPConnection::Connect()
{
    if (m_socket->Join(m_address))
    {
        m_socket->SetBlocking(false);
        return true;
    }
    return false;
}

//*******************************************************************
bool TCPConnection::IsDisconnected()
{
    if ((m_socket == nullptr) || (!m_socket->IsValid()))
        return true;
    return false;
}
