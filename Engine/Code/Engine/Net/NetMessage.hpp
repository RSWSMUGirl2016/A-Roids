#pragma once
#include "Engine/EngineCommon.hpp"
#include "Engine/MeshBuilder/FileBinaryStream.hpp"

class NetConnection;

enum eCoreNetMessage : uint8_t
{
    JOIN_RESPONSE = 0,
    NUM_CORE_MESSAGES = 32,
};

enum eMessageId : uint8_t
{
    SEND_COMMAND = NUM_CORE_MESSAGES,
    SEND_MESSAGE,
    NUM_MESSAGES, 
};


class NetMessage : public FileBinaryStream
{
public:
    NetMessage();
    NetMessage(uint8_t type_index);
    ~NetMessage();

    virtual uint write_bytes( void const *data, uint size ) override;
    virtual uint read_bytes( void *out, uint const max_size ) override;

    std::string ReadString();
    void WriteString(const std::string& message);

public:
    uint8_t m_messageTypeIndex;
    NetConnection *m_sender;
    byte_t m_payload[1024];
    uint m_payloadBytesUsed = 0;
    bool m_executingMessage;
};