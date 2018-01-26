#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHITypes.hpp"
#include "Engine/Core/Rgba.hpp"

class RHIDevice;

struct sampler_desc_t
{
    sampler_desc_t()
        : m_minMipMap(0.0f)
        , m_maxMipMap(MAX_FLOAT)
        , m_borderColor(Rgba::BLACK)
        , m_anistropyAmount(1U)
        , m_uWrap(WRAPMODE_WRAP)
        , m_vWrap(WRAPMODE_WRAP)
        , m_wWrap(WRAPMODE_WRAP)
        , m_minFilter(FILTERMODE_LINEAR)
        , m_magFilter(FILTERMODE_LINEAR)
        , m_mipFilter(FILTERMODE_NONE)
    {};

    void SetWrapMode(eWrapMode uwrap, eWrapMode vwrap, eWrapMode wwrap);
    void SetWrapMode(eWrapMode wrap);
    void SetBorderColor(Rgba const &color);
    void SetFilter(eFilterMode minFilter, eFilterMode magFilter, eFilterMode mipFilter);
    inline void SetFilter(eFilterMode const filter) { SetFilter(filter, filter, filter); }
    void SetFilterAnisotropic(unsigned int amount);

    float m_minMipMap; // 0 being most detailed.
    float m_maxMipMap; // 0 being most detailed.  (defaults to max)
    Rgba m_borderColor; // only used if wrap mode is border.
    unsigned int m_anistropyAmount; // default one - only applies if using anistropic filtering
    eWrapMode m_uWrap;       // How it wraps when U leaves the [0, 1] region.
    eWrapMode m_vWrap;       // ditto for V
    eWrapMode m_wWrap;       // ditto for W
    eFilterMode m_minFilter; // how it behaves when a pixel contains multiple texels
    eFilterMode m_magFilter; // how it behaves when a texel covers multiple pixels
    eFilterMode m_mipFilter; // how it behaves when crossing mip boundaries. 
};

class Sampler
{
public:
    Sampler(RHIDevice *device);
    Sampler(RHIDevice *device, eFilterMode minFilter, eFilterMode magFilter);
    Sampler(RHIDevice *device, sampler_desc_t const *optionalDescription);
    ~Sampler();
    bool CreateSampler(eFilterMode minFilter, eFilterMode magFilter);
    inline bool IsValid() const { return (nullptr != m_dxSampler); }

public:
    RHIDevice *m_device;
    ID3D11SamplerState *m_dxSampler;
    sampler_desc_t m_desc;
};