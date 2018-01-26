#include "Engine/RHI/BlendState.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/RHI/RHIDevice.hpp"

static D3D11_BLEND ToDXFactor(eBlendFactor const factor)
{
    switch (factor)
    {
    case BLEND_ONE:
        return D3D11_BLEND_ONE;

    case BLEND_ZERO:
        return D3D11_BLEND_ZERO;

    case BLEND_SRC_ALPHA:
        return D3D11_BLEND_SRC_ALPHA;

    case BLEND_INV_SRC_ALPHA:
        return D3D11_BLEND_INV_SRC_ALPHA;
    
    default:
            //assert_or_die(false)
        return D3D11_BLEND_ONE;
    }
}

//*******************************************************************
BlendState::BlendState(RHIDevice *device, bool enabled, eBlendFactor const srcFactor /*= BLEND_ONE*/, eBlendFactor const destFactor /*= BLEND_ZERO*/)
    : m_device(device)
    , m_dxState(nullptr)
{
    D3D11_BLEND_DESC desc;
    MemZero(&desc);

    desc.AlphaToCoverageEnable = false; // multisampling
    desc.IndependentBlendEnable = false; //multiple render target = do we have different blend state per?

    desc.RenderTarget[0].BlendEnable = enabled;

    desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlend = ToDXFactor(srcFactor);
    desc.RenderTarget[0].DestBlend = ToDXFactor(destFactor);

    desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

    desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    HRESULT hr = device->m_dxDevice->CreateBlendState(&desc, &m_dxState);
    if (FAILED(hr))
    {
        //assert_or_die
        m_dxState = nullptr;
        DebuggerPrintf("Failed to compile in BlendState.\n");
    }
}

BlendState::~BlendState()
{
    if (m_dxState != nullptr)
    {
        m_dxState->Release();
        m_dxState = nullptr;
    }
}


