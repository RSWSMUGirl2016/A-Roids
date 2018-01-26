#include "Engine/RHI/DepthStencilState.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/RHI/RHIDevice.hpp"

//*******************************************************************
DepthStencilState::DepthStencilState(RHIDevice *device, depth_stencil_desc_t const &desc)
    : m_device(device)
    , m_dxState(nullptr)
{
    D3D11_DEPTH_STENCIL_DESC dxdesc;
    MemZero(&dxdesc);

    dxdesc.DepthEnable = desc.depth_writing_enabled || desc.depth_test_enabled;
    dxdesc.DepthWriteMask = desc.depth_writing_enabled ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
    dxdesc.DepthFunc = desc.depth_test_enabled ? D3D11_COMPARISON_LESS : D3D11_COMPARISON_ALWAYS;
    dxdesc.StencilEnable = FALSE;

    HRESULT hr = device->m_dxDevice->CreateDepthStencilState(&dxdesc, &m_dxState);
    if (FAILED(hr))
    {
        //assert_or_die
        m_dxState = nullptr;
        DebuggerPrintf("Failed to compile in DepthStencilState.\n");
    }
}

DepthStencilState::~DepthStencilState()
{
     m_dxState->Release();
     m_dxState = nullptr;
}


