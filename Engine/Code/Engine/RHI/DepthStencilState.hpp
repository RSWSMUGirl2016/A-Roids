#pragma once

#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHITypes.hpp"

class RHIDevice;

struct depth_stencil_desc_t
{
    bool depth_writing_enabled;
    bool depth_test_enabled;
};

class DepthStencilState
{
public:
    DepthStencilState(RHIDevice *device, depth_stencil_desc_t const &desc);
    ~DepthStencilState();
    
    inline bool IsValid() const { return (nullptr != m_dxState); }

public:
    RHIDevice *m_device;
    ID3D11DepthStencilState *m_dxState;

};