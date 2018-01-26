#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/RHI/RasterState.hpp"
#include "Engine/RHI/RHIDevice.hpp"

//*******************************************************************
RasterState::RasterState(RHIDevice *device)
    : m_device(device)
    , m_dxState(nullptr)
{
    D3D11_RASTERIZER_DESC desc;
    MemZero(&desc);

    desc.FillMode = D3D11_FILL_SOLID; //D3D_FILL_WIRE
    desc.CullMode = D3D11_CULL_BACK;

    //Make it act like OpenGL
    desc.FrontCounterClockwise = true;

    desc.AntialiasedLineEnable = false;
    desc.DepthBias = (int)0.0f;
    desc.DepthBiasClamp = 0.0f;
    desc.SlopeScaledDepthBias = 0.0f;
    desc.DepthClipEnable = true;
    desc.ScissorEnable = false;
    desc.MultisampleEnable = false;

    HRESULT hr = device->m_dxDevice->CreateRasterizerState(&desc, &m_dxState);
    if (FAILED(hr)) 
    {
        DebuggerPrintf("Failed to compile in RasterState.\n");
    }
}

//*******************************************************************
RasterState::~RasterState()
{
    m_dxState->Release();
    m_dxState = nullptr;
}
