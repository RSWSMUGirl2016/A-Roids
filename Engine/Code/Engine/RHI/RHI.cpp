#include "Engine/RHI/RHI.hpp"

void RHI::RHIStartup()
{
	m_rhiInstance->GetInstance();
}

void RHI::RHIShutdown()
{
	delete m_rhiInstance;
	m_rhiInstance = nullptr;
}
