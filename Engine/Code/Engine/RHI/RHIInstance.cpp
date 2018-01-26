#pragma once
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHIInstance.hpp"
#include "Engine/Window.hpp"
#include "Engine/RHI/RHIOutput.hpp"
#include "Game/GameCommon.hpp"

RHIInstance* RHIInstance::m_rhiInstance = nullptr;

RHIInstance::~RHIInstance()
{

}

RHIOutput* RHIInstance::CreateOutput(WindowMessageHandler windowMessageHandler)

{
	unsigned int deviceFlags = D3D11_CREATE_DEVICE_DEBUG;

	Window* window = new Window();

    //int windowResolutionX;
    //int windowResolutionY;
    //window->m_gameConfig.GetWindowResolutionX(windowResolutionX);
    //window->m_gameConfig.GetWindowResolutionY(windowResolutionY);

    //window->SetClientSize(windowResolutionX, windowResolutionY);
    window->SetClientSize((unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT);

	window->m_windowMessageHandler = windowMessageHandler;
	window->Open();

	DXGI_SWAP_CHAIN_DESC swapDesc;
	memset(&swapDesc, 0, sizeof(swapDesc));

	swapDesc.BufferCount = 2;

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_BACK_BUFFER;
	swapDesc.OutputWindow = window->m_hwnd;
	swapDesc.SampleDesc.Count = 1;


	swapDesc.Windowed = TRUE;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.Width = (unsigned int)WINDOW_WIDTH;
	swapDesc.BufferDesc.Height = (unsigned int)WINDOW_HEIGHT;

	ID3D11Device *dx_D3DDevice = nullptr;
	ID3D11DeviceContext *dx_D3DContext = nullptr;
	IDXGISwapChain *dx_D3DSwapChain = nullptr;

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, deviceFlags, nullptr, 0U, D3D11_SDK_VERSION, &swapDesc, &dx_D3DSwapChain, &dx_D3DDevice, nullptr, &dx_D3DContext);

	bool success = SUCCEEDED(hr);
	if (success)
	{
		RHIDevice* device = new RHIDevice();
		device->m_dxDevice = dx_D3DDevice;
		RHIDeviceContext* deviceContext = new RHIDeviceContext(device, dx_D3DContext);
		device->m_immediateContext = deviceContext;

		RHIOutput* output = new RHIOutput(dx_D3DSwapChain, device, window);
        output->SetWidth(WINDOW_WIDTH);
        output->SetHeight(WINDOW_HEIGHT);
		return output;
	}
	else
	{
		delete window;
		window = nullptr;
	}
	return nullptr;
}

RHIInstance::RHIInstance()
{
}

RHIInstance* RHIInstance::GetInstance()
{
	if (m_rhiInstance)
	{		
		return m_rhiInstance;
	}
	else
	{
		m_rhiInstance = new RHIInstance();
		return m_rhiInstance;
	}
}
