#include "Engine/EngineConfig.hpp"


EngineConfig::EngineConfig()
{
	m_config = Config("Engine/EngineConfig.config");
}

EngineConfig::~EngineConfig()
{
}

void EngineConfig::ListMembers()
{
	m_config.ConfigList();
}

