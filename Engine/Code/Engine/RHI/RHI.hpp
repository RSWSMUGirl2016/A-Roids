#pragma once

#include "Engine/RHI/RHI.hpp"
#include "Engine/RHI/RHITypes.hpp"
#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHIOutput.hpp"
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/RHI/ShaderProgram.hpp"

class RHI
{
public:
	RHIInstance* m_rhiInstance;

	void RHIStartup();
	void RHIShutdown();
};