#pragma once
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/Window.hpp"
#include "Engine/RHI/DX11.hpp"

class RHIOutput
{
public:
	RHIOutput(IDXGISwapChain *swapChain, RHIDevice *owner, Window *window);
	~RHIOutput();
	void Present();
	void Close();
	Texture2D* GetRenderTarget();
	float GetWidth() const;
	float GetHeight() const;
	float GetAspectRatio() const;
    void SetWidth(float width);
    void SetHeight(float height);
public:
	RHIDevice *m_device;
	Window *m_window;
	IDXGISwapChain *m_swapChain;
	Texture2D *m_renderTarget;

};
