#include "Engine/Net/NetMessageDefinition.hpp"

//*******************************************************************
void NetMessageDefinition::SetFunctionHandler(std::function<void(NetMessage*)> const &callback)
{
    m_functionHandler = callback;
}
