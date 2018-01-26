#pragma once
#include "Engine/Core/Rgba.hpp"
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RasterState.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/RHI/BlendState.hpp"
#include "Engine/RHI/DepthStencilState.hpp"
#include "Engine/MeshBuilder/StructuredBuffer.hpp"

class ComputeJob;

class RHIDeviceContext
{
public:
	RHIDeviceContext(RHIDevice *owner, ID3D11DeviceContext *ctx);
	~RHIDeviceContext();
	void ClearState();
	void Flush();   

	void ClearColorTarget(Texture2D* output, Rgba const &color);    
    void ClearDepthTarget(Texture2D *output, float depth = 1.0f, uint8_t stencil = 0);

    void SetRasterState(RasterState *rs);
    void SetBlendState(BlendState *bs);
    void SetDepthStencilState(DepthStencilState *dss);

    void SetColorTarget(RHIOutput *output, Texture2D *target);
    void SetColorTarget(Texture2D *colorTarget, Texture2D *depthTarget = nullptr);

    void SetViewport(unsigned int x, unsigned y, unsigned int width, unsigned int height);

    void SetShaderProgram(ShaderProgram *shaderProgram);
    void SetComputeShader(ComputeShader *computeShader);
    void SetTexture(unsigned int const idx, Texture2D *texture);
    void SetUavTexture(unsigned int const idx, Texture2D *texture);
    void SetSampler(unsigned const idx, Sampler *sampler);

    void Draw(ePrimitiveType const topology, VertexBuffer *vbo, unsigned const vertexCount, unsigned int const startIdx = 0U);
    void DrawIndexed(ePrimitiveType const topology, VertexBuffer *vbo, IndexBuffer *ibo, unsigned const indexCount, unsigned int const startIdx = 0U);

    void SetConstantBuffer(unsigned int idx, ConstantBuffer *cb);
    void SetCSConstantBuffer(unsigned int idx, ConstantBuffer *cb);
    void SetStructuredBuffer(unsigned int bindPoint, StructuredBuffer *sb);

    void Dispatch(ComputeJob *job);

public:
	RHIDevice *m_device;
    ID3D11DeviceContext *m_dxContext;
    Texture2D *m_currentTarget;
    RasterState *m_rasterState;
    VertexBuffer *m_tempVbo;

    Texture2D* m_currentTexture = nullptr;
};