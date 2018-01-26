#pragma once
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/RHI/RHIDevice.hpp"

#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHIOutput.hpp"

#include "Engine/Core/ErrorWarningAssert.hpp"


//*******************************************************************
Texture2D::Texture2D(RHIDevice *device)
    : m_device(device)
    , m_dxResource(nullptr)
    , m_renderTargetView(nullptr)
    , m_width(0)
    , m_height(0)
    , m_dxBindFlags(0U)
    , m_dxSrv(nullptr)
    , m_dxDsv(nullptr)
    , m_dxUav(nullptr)
{
}

//*******************************************************************
Texture2D::Texture2D(RHIDevice *device, RHIOutput *output)
    : Texture2D(device)
{
    if (output != nullptr) {
        ID3D11Texture2D *backBuffer = nullptr;
        output->m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
        if (nullptr != backBuffer) {
            m_dxResource = backBuffer;

            D3D11_TEXTURE2D_DESC desc;
            m_dxResource->GetDesc(&desc);

            m_width = desc.Width;
            m_height = desc.Height;
            m_dxBindFlags = desc.BindFlags;

            CreateViews();
        }
    }
}

//*******************************************************************
Texture2D::Texture2D(RHIDevice *device, Image const &image)
    : Texture2D(device)
{
    LoadFromImage(image);
}

//*******************************************************************
Texture2D::Texture2D(RHIDevice *device, unsigned int width, unsigned int height, eImageFormat format, bool isUav)
    : Texture2D(device)
{
    m_isUav = isUav;

    // default usage - this is going to be written to by the GPU
    D3D11_USAGE usage = D3D11_USAGE_DEFAULT;
    m_width = width;
    m_height = height;

    DXGI_FORMAT dx_format;
    unsigned int dx_bindings = 0U;
    switch (format) {
    case IMAGEFORMAT_RGBA8:
        dx_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dx_bindings = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        break;
    case IMAGEFORMAT_D24S8:
        // depth textures are 24-bit depth, 8-bit stencil
        dx_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dx_bindings = D3D11_BIND_DEPTH_STENCIL; // binding this to as a shader resource requires a couple extra steps - saved for later
        break;
    default:
        // ASSERTORDIE
        return;
    };

    D3D11_TEXTURE2D_DESC tex_desc;
    memset(&tex_desc, 0, sizeof(tex_desc));

    tex_desc.Width = width;
    tex_desc.Height = height;
    tex_desc.MipLevels = 1;                // no mip mapping
    tex_desc.ArraySize = 1;                // NO ARRAYS!  
    tex_desc.Usage = usage;
    tex_desc.Format = dx_format;
    tex_desc.BindFlags = dx_bindings;
    tex_desc.CPUAccessFlags = 0U;
    tex_desc.MiscFlags = 0U; // there is one for generating mip maps.

                             // multisampling options
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;

    // no initial data - we're creating renderable targets

    ID3D11Device *dx_device = device->m_dxDevice;
    HRESULT hr = dx_device->CreateTexture2D(&tex_desc, nullptr, &m_dxResource);

    if (SUCCEEDED(hr)) {
        m_dxBindFlags = dx_bindings;
        CreateViews();
    }
}

//*******************************************************************
void Texture2D::Destroy()
{
    if (IsValid() && IsRenderTarget()) {
        m_renderTargetView->Release();
        m_renderTargetView = nullptr;

        m_dxResource->Release();
        m_dxResource = nullptr;
    }
}

//*******************************************************************
Texture2D::Texture2D(RHIDevice *device, char const *filename, bool isUav)
    : Texture2D(device)
{
    m_isUav = isUav;

    Image image;
    if (!image.LoadFromFile(filename)) {
        return;
    }

    LoadFromImage(image);
}

//*******************************************************************
Texture2D::Texture2D(RHIDevice *device, ID3D11Texture2D *tex)
    :m_device(device)
    , m_dxResource(tex)
    , m_dxSrv(nullptr)
    , m_renderTargetView(nullptr)
    , m_width(0)
    , m_height(0)
    , m_dxBindFlags(0U)
    , m_dxDsv(nullptr)
    , m_dxUav(nullptr)
{
    HRESULT hr = m_device->m_dxDevice->CreateRenderTargetView(m_dxResource, nullptr, &m_renderTargetView);

    m_dxBindFlags = D3D11_BIND_RENDER_TARGET;
    if (SUCCEEDED(hr))
    {
        CreateViews();
    }
    else
    {
        ASSERT_OR_DIE(false, "Creating Render Target View failed");
    }
}

//*******************************************************************
Texture2D::~Texture2D()
{
    if (m_dxSrv != nullptr)
    {
        m_dxSrv->Release();
        m_dxSrv = nullptr;
    }

    if (m_dxDsv != nullptr)
    {
        m_dxDsv->Release();
        m_dxDsv = nullptr;
    }

    if (m_dxUav != nullptr)
    {
        m_dxUav->Release();
        m_dxUav = nullptr;
    }

    /*if (m_device != nullptr)
    {
        delete m_device;
        m_device = nullptr;
    }*/

    Destroy();
}

//*******************************************************************
bool Texture2D::LoadFromImage(Image const& image)
{
    D3D11_USAGE usage = D3D11_USAGE_IMMUTABLE;
    if (m_isUav)
        usage = D3D11_USAGE_DEFAULT;

    D3D11_TEXTURE2D_DESC tex_desc;
    memset(&tex_desc, 0, sizeof(tex_desc));

    tex_desc.Width = image.GetWidth();
    tex_desc.Height = image.GetHeight();
    tex_desc.MipLevels = 1;
    tex_desc.ArraySize = 1;
    tex_desc.Usage = usage;
    tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    if (m_isUav)
        tex_desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;

    tex_desc.CPUAccessFlags = 0U;
    tex_desc.MiscFlags = 0U;

    //multisampling
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;

    m_dxBindFlags = D3D11_BIND_SHADER_RESOURCE;
    if (m_isUav)
        m_dxBindFlags |= D3D11_BIND_UNORDERED_ACCESS;

    D3D11_SUBRESOURCE_DATA data;
    memset(&data, 0, sizeof(data));

    data.pSysMem = image.m_buffer;
    data.SysMemPitch = image.m_bpp * tex_desc.Width;

    ID3D11Device *dx_device = m_device->m_dxDevice;
    HRESULT hr = dx_device->CreateTexture2D(&tex_desc, &data, &m_dxResource);

    if (SUCCEEDED(hr))
    {
        CreateViews();
        return true;
    }
    else
    {
        return false;
    }
}

//*******************************************************************
unsigned int Texture2D::GetWidth()
{
    return m_width;
}

//*******************************************************************
unsigned int Texture2D::GetHeight()
{
    return m_height;
}

//*******************************************************************
void Texture2D::SetWidth(unsigned int width)
{
    m_width = width;
}

//*******************************************************************
void Texture2D::SetHeight(unsigned int height)
{
    m_height = height;
}

//*******************************************************************
void Texture2D::CreateViews()
{
    ID3D11Device *dd = m_device->m_dxDevice;

    if (m_dxBindFlags & D3D11_BIND_RENDER_TARGET)
    {
        HRESULT hr = dd->CreateRenderTargetView(m_dxResource, nullptr, &m_renderTargetView);
        if (FAILED(hr))
        {
            ASSERT_OR_DIE(false, "Create Render Target View failed");
        }
    }

    if (m_dxBindFlags & D3D11_BIND_SHADER_RESOURCE)
    {
        HRESULT hr = dd->CreateShaderResourceView(m_dxResource, nullptr, &m_dxSrv);
        if (FAILED(hr))
        {
            ASSERT_OR_DIE(false, "Create Shader Resource View failed");
        }
    }

    if (m_dxBindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        HRESULT hr = dd->CreateDepthStencilView(m_dxResource, nullptr, &m_dxDsv);
        if (FAILED(hr))
        {
            ASSERT_OR_DIE(false, "Create Depth Stencil View failed");
        }
    }

    if (m_dxBindFlags & D3D11_BIND_UNORDERED_ACCESS)
    {
        HRESULT hr = dd->CreateUnorderedAccessView(m_dxResource, nullptr, &m_dxUav);
        if (FAILED(hr))
        {
            ASSERT_OR_DIE(false, "Create Unordered Access View failed");
        }
    }
}