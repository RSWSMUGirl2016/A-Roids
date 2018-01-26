#pragma once

#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHITypes.hpp"

class RHIDevice;

class RasterState
{
public:
    RasterState(RHIDevice *device);
    ~RasterState();
    
    inline bool IsValid() const { return (nullptr != m_dxState); }

public:
    RHIDevice *m_device;
    ID3D11RasterizerState *m_dxState;
};