#pragma once
#include "Engine/Core/Image.hpp"
#include "Engine/RHI/DX11.hpp"

class RHIDevice;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;

class RHIDeviceContext;
class RHIOutput;
class Image;

class Texture2D
{
public:
    Texture2D(RHIDevice *device);
    Texture2D(RHIDevice *deivce, RHIOutput *output);
    Texture2D(RHIDevice *deivce, char const *filename, bool isUav = false);
    Texture2D(RHIDevice *deivce, Image const &image);

	Texture2D(RHIDevice *device, ID3D11Texture2D *tex);
    Texture2D(RHIDevice *device, unsigned int width, unsigned int height, eImageFormat format, bool isUav = false);
	~Texture2D();

    bool LoadFromImage(Image const &image);
    void Destroy();
	unsigned int GetWidth();
	unsigned int GetHeight();
    void SetWidth(unsigned int width);
    void SetHeight(unsigned int height);
	inline bool IsValid() const { return (m_dxResource != nullptr); }
	inline bool IsRenderTarget() const { return (m_renderTargetView != nullptr); }
    void CreateViews();

public:
	RHIDevice *m_device;

	ID3D11Texture2D *m_dxResource;
	ID3D11ShaderResourceView *m_dxSrv;
    ID3D11UnorderedAccessView *m_dxUav;
    ID3D11RenderTargetView *m_renderTargetView;
    ID3D11DepthStencilView *m_dxDsv;

	unsigned int m_width;
	unsigned int m_height;
    unsigned int m_dxBindFlags;
    bool m_isUav;
};