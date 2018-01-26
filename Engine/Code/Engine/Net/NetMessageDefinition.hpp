#pragma once
#include "Engine/Threading/Event.hpp"
#include "Engine/Net/NetMessage.hpp"

class NetMessageDefinition
{
public:
    uint8_t m_typeIndex;
    std::function<void(NetMessage*)> m_functionHandler;

    void SetFunctionHandler(std::function<void(NetMessage*)> const &callback);
};