#include "Engine/RHI/Sampler.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
Sampler::Sampler(RHIDevice *device)
    : m_device(device)
    , m_dxSampler(nullptr)
{}

//*******************************************************************
Sampler::Sampler(RHIDevice *device, eFilterMode min, eFilterMode mag)
    : Sampler(device)
{
    CreateSampler(min, mag);
}

//*******************************************************************
Sampler::Sampler(RHIDevice *device, sampler_desc_t const *optionalDescription)
    : m_device(device)
    , m_desc(*optionalDescription)
{
    CreateSampler(m_desc.m_minFilter, m_desc.m_magFilter);
}

//*******************************************************************
Sampler::~Sampler()
{
    /*if (m_device != nullptr)
    {
        delete m_device;
        m_device = nullptr;
    }*/
    
    if (m_dxSampler != nullptr)
    {
        m_dxSampler->Release();
        m_dxSampler = nullptr;
    }    
}

//*******************************************************************
static D3D11_FILTER DXGetFilterMode(eFilterMode min, eFilterMode mag)
{
    if (min == FILTER_POINT) {
        if (mag == FILTER_POINT) {
            return D3D11_FILTER_MIN_MAG_MIP_POINT;
        }
        else { // LINEAER
            return D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
        }
    }
    else { // assume linear
        if (mag == FILTER_POINT) {
            return D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
        }
        else { // assume linear
            return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
        }
    }
}

//*******************************************************************
bool Sampler::CreateSampler(eFilterMode min, eFilterMode mag)
{
    D3D11_SAMPLER_DESC desc;
    MemZero(&desc);

    desc.Filter = DXGetFilterMode(min, mag);
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    // desc.BorderColor = BLACK TRANSPARENT if 0.
    desc.MinLOD = 0;
    desc.MaxLOD = 0;

    ID3D11Device *dx_device = m_device->m_dxDevice;
    HRESULT hr = dx_device->CreateSamplerState(&desc, &m_dxSampler);

    return SUCCEEDED(hr);
}

//*******************************************************************
void sampler_desc_t::SetWrapMode(eWrapMode uwrap, eWrapMode vwrap, eWrapMode wwrap)
{
    m_uWrap = uwrap;
    m_vWrap = vwrap;
    m_wWrap = wwrap;
}

//*******************************************************************
void sampler_desc_t::SetWrapMode(eWrapMode wrap)
{
    m_uWrap = wrap;
    m_vWrap = wrap;
    m_wWrap = wrap;
}

//*******************************************************************
void sampler_desc_t::SetBorderColor(Rgba const &color)
{
    m_borderColor = color;
}

//*******************************************************************
void sampler_desc_t::SetFilter(eFilterMode minFilter, eFilterMode magFilter, eFilterMode mipFilter)
{
    m_minFilter = minFilter;
    m_magFilter = magFilter;
    m_mipFilter = mipFilter;
}

//*******************************************************************
void sampler_desc_t::SetFilterAnisotropic(unsigned int amount)
{
    m_anistropyAmount = amount;
}
