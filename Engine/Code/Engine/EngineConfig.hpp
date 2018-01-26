#pragma once

#include "Engine/Config.hpp"

class EngineConfig
{
public:
	EngineConfig();
	~EngineConfig();

	void ListMembers();

private:
	Config m_config;
};