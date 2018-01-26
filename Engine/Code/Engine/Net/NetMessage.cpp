#include "Engine/Net/NetMessage.hpp"
#include "Engine/Net/NetConnection.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
NetMessage::NetMessage()
    : m_executingMessage(false)
{}

//*******************************************************************
NetMessage::NetMessage(uint8_t type_index)
    : m_messageTypeIndex(type_index)
    , m_executingMessage(false)
{}

//*******************************************************************
NetMessage::~NetMessage()
{}

//*******************************************************************
uint NetMessage::write_bytes(void const *data, uint size)
{
    //DebuggerPrintf("Data Size: %i", size);
    //Append, not at the start
    ASSERT_OR_DIE(size < (1024 - m_payloadBytesUsed), "NetMessage::write_bytes, size is greater than payload size.");

    uint bytesWritten = size;
    for (uint index = 0; index < size; ++index) {
        m_payload[m_payloadBytesUsed] = ((byte_t*)data)[index];
        m_payloadBytesUsed++;
    }

    return bytesWritten;
}

//*******************************************************************
uint NetMessage::read_bytes(void *out, uint const max_size)
{
    ASSERT_OR_DIE(max_size < 1024, "NetMessage::read_bytes, max_size is greater than payload size.");

    uint bytesRead = max_size;
    for (uint index = 0; index < max_size; ++index) {
        out = (void *)m_payload[index];
    }
    return bytesRead;
}

//*******************************************************************
std::string NetMessage::ReadString()
{
    uint16_t sizeOfString = *(uint16_t*)m_payload;
    if (sizeOfString == 0xffff)
        return std::string();

    std::string str;
    str.resize(sizeOfString);
    std::memcpy((void*)str.data(), (void*)(m_payload + 2), sizeOfString);
    return str;
}

//*******************************************************************
void NetMessage::WriteString(const std::string& message)
{
    uint16_t newSize = (uint16_t)message.size();
    if (newSize > 0xffff)
        newSize = 0xfffe;
    
    *(uint16_t*)m_payload = newSize;

    m_payloadBytesUsed = newSize + 2;
    std::memcpy((void*)(m_payload + 2), message.data(), newSize);
}
