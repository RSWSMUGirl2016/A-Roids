#pragma once
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/RHITypes.hpp"
#include "Engine/Math/Vertex3.hpp"
#include "Engine/rhi/VertexBuffer.hpp"
#include "Engine/RHI/IndexBuffer.hpp"
#include "Engine/RHI/ComputeJob.hpp"
#include "Engine/Input/InputSystem.hpp"

//*******************************************************************
RHIDeviceContext::RHIDeviceContext(RHIDevice *owner, ID3D11DeviceContext *ctx)
    : m_device(owner)
    , m_dxContext(ctx)
    , m_currentTarget(nullptr)
    , m_rasterState(nullptr)
    , m_tempVbo(nullptr)
{
}

//*******************************************************************
RHIDeviceContext::~RHIDeviceContext()
{
    /*if (m_device != nullptr)
    {
        delete m_device;
        m_device = nullptr;
    }*/

    if (m_dxContext != nullptr)
    {
        m_dxContext->Release();
        m_dxContext = nullptr;
    }

    if (m_currentTarget != nullptr)
    {
        delete m_currentTarget;
        m_currentTarget = nullptr;
    }

    if (m_rasterState != nullptr)
    {
        delete m_rasterState;
        m_rasterState = nullptr;
    }

    if (m_tempVbo != nullptr)
    {
        delete m_tempVbo;
        m_tempVbo = nullptr;
    }
}

//*******************************************************************
void RHIDeviceContext::ClearState()
{
    m_dxContext->ClearState();
}

//*******************************************************************
void RHIDeviceContext::Flush()
{
    m_dxContext->Flush();
}

//*******************************************************************
void RHIDeviceContext::ClearColorTarget(Texture2D* output, Rgba const &color)
{
    float clearColor[4];
    color.GetAsFloats(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);
    m_dxContext->ClearRenderTargetView(output->m_renderTargetView, clearColor);
}

//*******************************************************************
void RHIDeviceContext::ClearDepthTarget(Texture2D *output, float depth /*= 1.0f*/, uint8_t stencil /*=0.f*/)
{
    if (output != nullptr)
    {
        m_dxContext->ClearDepthStencilView(output->m_dxDsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
    }
}

//*******************************************************************
void RHIDeviceContext::SetRasterState(RasterState *rs)
{
    m_dxContext->RSSetState(rs->m_dxState);
}

//*******************************************************************
void RHIDeviceContext::SetBlendState(BlendState *bs)
{
    float constant[] = { 1, 1, 1, 1 };
    m_dxContext->OMSetBlendState(bs->m_dxState, constant, 0xffffffff);
}

void RHIDeviceContext::SetDepthStencilState(DepthStencilState *dss)
{
    m_dxContext->OMSetDepthStencilState(dss->m_dxState, 0U);
}

//*******************************************************************
void RHIDeviceContext::SetColorTarget(RHIOutput *output, Texture2D *target)
{
    if (target != nullptr)
    {
        m_currentTarget = target;
    }
    else
    {
        m_currentTarget = output->GetRenderTarget();
    }

    m_dxContext->OMSetRenderTargets(1, &m_currentTarget->m_renderTargetView, nullptr);
    /*m_dxContext->OMSetRenderTargets(1, &m_currentTarget->m_renderTargetView, depthStencilTarget != nullptr ? depthStencilTarget->m_dxDsv : nullptr);*/
}

//*******************************************************************
void RHIDeviceContext::SetColorTarget(Texture2D *colorTarget, Texture2D *depthTarget /*= nullptr*/)
{
    m_dxContext->OMSetRenderTargets(1, &colorTarget->m_renderTargetView, depthTarget != nullptr ? depthTarget->m_dxDsv : nullptr);
}

//*******************************************************************
void RHIDeviceContext::SetViewport(unsigned int x, unsigned y, unsigned int width, unsigned int height)
{
    D3D11_VIEWPORT viewport;
    memset(&viewport, 0, sizeof(viewport));
    viewport.TopLeftX = (FLOAT)x;
    viewport.TopLeftY = (FLOAT)y;
    viewport.Width = (FLOAT)width;
    viewport.Height = (FLOAT)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    m_dxContext->RSSetViewports(1, &viewport);
}

//*******************************************************************
void RHIDeviceContext::SetShaderProgram(ShaderProgram *shaderProgram)
{
    m_dxContext->VSSetShader(shaderProgram->m_dxVertexShader, nullptr, 0U);
    m_dxContext->PSSetShader(shaderProgram->m_dxFragmentShader, nullptr, 0U);
    m_dxContext->IASetInputLayout(shaderProgram->m_dxInputLayout);
}

//*******************************************************************
void RHIDeviceContext::SetComputeShader(ComputeShader *computeShader)
{
    m_dxContext->CSSetShader(computeShader->m_dxComputeShader, nullptr, 0);
}

//*******************************************************************
void RHIDeviceContext::SetTexture(unsigned int const idx, Texture2D *texture)
{
    m_dxContext->VSSetShaderResources(idx, 1, &texture->m_dxSrv);
    m_dxContext->PSSetShaderResources(idx, 1, &texture->m_dxSrv);
    m_currentTexture = texture;
}

//*******************************************************************
void RHIDeviceContext::SetUavTexture(unsigned int const idx, Texture2D *texture)
{
    m_dxContext->CSSetUnorderedAccessViews(idx, 1, &texture->m_dxUav, nullptr);
    m_currentTexture = texture;
}

//*******************************************************************
void RHIDeviceContext::SetSampler(unsigned const idx, Sampler *sampler)
{
    m_dxContext->VSSetSamplers(idx, 1, &sampler->m_dxSampler);
    m_dxContext->PSSetSamplers(idx, 1, &sampler->m_dxSampler);
}

//*******************************************************************
void RHIDeviceContext::Draw(ePrimitiveType const topology, VertexBuffer *vbo, unsigned const vertexCount, unsigned int const startIdx /*= 0U*/)
{
    UNUSED(startIdx);

    D3D11_PRIMITIVE_TOPOLOGY d3dPrim;
    switch (topology) {
    case ePrimitiveType::PRIMITIVE_TRIANGLES:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    case ePrimitiveType::PRIMITIVE_TRIANGLE_STRIP:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        break;
    case ePrimitiveType::PRIMITIVE_LINES:
        d3dPrim = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    case ePrimitiveType::PRIMITIVE_LINE_STRIP:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        break;
    default:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }

    m_dxContext->IASetPrimitiveTopology(d3dPrim);
    unsigned int stride = sizeof(Vertex3);
    unsigned int offsets = 0;
    m_dxContext->IASetVertexBuffers(0, 1, &vbo->m_dxBuffer, &stride, &offsets);
    m_dxContext->Draw(vertexCount, 0);
}

//*******************************************************************
void RHIDeviceContext::DrawIndexed(ePrimitiveType const topology, VertexBuffer *vbo, IndexBuffer *ibo, unsigned const indexCount, unsigned int const startIdx /*= 0U*/)
{
    UNUSED(startIdx);

    D3D11_PRIMITIVE_TOPOLOGY d3dPrim;
    switch (topology) {
    case ePrimitiveType::PRIMITIVE_TRIANGLES:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    case ePrimitiveType::PRIMITIVE_TRIANGLE_STRIP:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        break;
    case ePrimitiveType::PRIMITIVE_LINES:
        d3dPrim = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    default:
        d3dPrim = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }

    m_dxContext->IASetPrimitiveTopology(d3dPrim);
    unsigned int stride = sizeof(Vertex3);
    unsigned int offsets = 0;
    m_dxContext->IASetVertexBuffers(0, 1, &vbo->m_dxBuffer, &stride, &offsets);
    m_dxContext->IASetIndexBuffer(ibo->m_dxBuffer, DXGI_FORMAT_R32_UINT, 0);
    m_dxContext->DrawIndexed(indexCount, 0, 0);
}

//*******************************************************************
void RHIDeviceContext::SetConstantBuffer(unsigned int idx, ConstantBuffer *cb)
{
    m_dxContext->VSSetConstantBuffers(idx, 1, &cb->m_dxBuffer);
    m_dxContext->PSSetConstantBuffers(idx, 1, &cb->m_dxBuffer);
}

//*******************************************************************
void RHIDeviceContext::SetCSConstantBuffer(unsigned int idx, ConstantBuffer *cb)
{
    m_dxContext->CSSetConstantBuffers(idx, 1, &cb->m_dxBuffer);
}

//*******************************************************************
void RHIDeviceContext::SetStructuredBuffer(unsigned int bindPoint, StructuredBuffer *sb)
{
    if (sb != nullptr)
    {
        m_dxContext->VSSetShaderResources(bindPoint, 1, &sb->m_dxSrv);
        m_dxContext->PSSetShaderResources(bindPoint, 1, &sb->m_dxSrv);
    }
}

//*******************************************************************
void RHIDeviceContext::Dispatch(ComputeJob *job)
{
    //Unbind normal texture
    ID3D11ShaderResourceView* srv = nullptr;
    m_dxContext->VSSetShaderResources(0, 1, &srv);
    m_dxContext->PSSetShaderResources(0, 1, &srv);

    //#TO_DO "Bind Shader"
    SetComputeShader(job->m_shaderToRun);
    SetUavTexture(0, job->m_texture);
    job->SetColor(Vector4(1.0f, 0.f, 0.f, 1.0f));
    SetCSConstantBuffer(COLOR_BUFFER_INDEX, job->m_color_cb);

    //#OPTIONAL_TO_DO "Bind Job State"

    m_dxContext->Dispatch(job->m_gridSize.x, job->m_gridSize.y, job->m_gridSize.z);

    //#OPTIONAL_TO_DO "Unbind Job State"
    ID3D11UnorderedAccessView* uav = nullptr;
    m_dxContext->CSSetUnorderedAccessViews(0, 1, &uav, nullptr);
}


