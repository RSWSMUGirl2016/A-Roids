#pragma once

#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHITypes.hpp"

class RHIDevice;

enum eBlendFactor
{
    BLEND_ONE,
    BLEND_ZERO, 

    BLEND_SRC_ALPHA,
    BLEND_INV_SRC_ALPHA,

    BLEND_SRC_COLOR,
    BLEND_DEST_COLOR,
};

class BlendState
{
public:
    BlendState(RHIDevice *device, bool enabled = true, eBlendFactor const srcFactor = BLEND_ONE, eBlendFactor const destFactor = BLEND_ZERO);
    ~BlendState();
    
    inline bool IsValid() const { return (nullptr != m_dxState); }

public:
    RHIDevice *m_device;
    ID3D11BlendState *m_dxState;

};