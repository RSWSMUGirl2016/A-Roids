#pragma once
#include "Engine/RHI/RHIOutput.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
#define DX_SAFE_RELEASE(dx_resource)   if ((dx_resource) != nullptr) { dx_resource->Release(); dx_resource = nullptr; }

//*******************************************************************
RHIOutput::RHIOutput(IDXGISwapChain *swapChain, RHIDevice *owner, Window *window)
	: m_swapChain(swapChain)
	, m_device(owner)
	, m_window(window)
    , m_renderTarget(nullptr)
{
	ID3D11Texture2D *backBuffer = nullptr;
	HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    if (FAILED(hr))
    {
        ASSERT_OR_DIE(false, "Get Buffer for swap chain failed.");
    }
	m_renderTarget = new Texture2D(m_device, backBuffer);
}

//*******************************************************************
RHIOutput::~RHIOutput()
{
    if (m_renderTarget != nullptr)
    {
        delete m_renderTarget;
        m_renderTarget = nullptr;
    }

    if (m_window != nullptr)
    {
        delete m_window;
        m_window = nullptr;
    }

    /*if (m_device != nullptr)
    {
        delete m_device;
        m_device = nullptr;
    }*/

    if (m_swapChain != nullptr)
    {
        m_swapChain->Release();
        m_swapChain = nullptr;
    }
}

//*******************************************************************
void RHIOutput::Present()
{
	m_swapChain->Present(0, 0);
}

//*******************************************************************
void RHIOutput::Close()
{
	delete m_window;
	m_window = nullptr;
}

//*******************************************************************
Texture2D* RHIOutput::GetRenderTarget()
{
	return m_renderTarget;
}

//*******************************************************************
float RHIOutput::GetWidth() const
{
	return (float)m_renderTarget->GetWidth();
}

//*******************************************************************
float RHIOutput::GetHeight() const
{
	return (float)m_renderTarget->GetHeight();
}

//*******************************************************************
float RHIOutput::GetAspectRatio() const
{
	return (float)(GetWidth() / GetHeight());
}

//*******************************************************************
void RHIOutput::SetWidth(float width)
{
    m_renderTarget->SetWidth((unsigned int) width);
}

//*******************************************************************
void RHIOutput::SetHeight(float height)
{
    m_renderTarget->SetHeight((unsigned int)height);
}
