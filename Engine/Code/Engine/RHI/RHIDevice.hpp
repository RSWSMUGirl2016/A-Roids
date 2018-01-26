#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHIOutput.hpp"
#include "Engine/RHI/Sampler.hpp"

class RHIDeviceContext;
class ShaderProgram;
class VertexBuffer;
class IndexBuffer;
class Vertex3;
class ComputeShader;

class RHIDevice
{
public:
    RHIDevice();
    ~RHIDevice();

    Texture2D* CreateTexture2DFromFile(char const *filename);
    Sampler* CreateSampler(sampler_desc_t const *optDesc);
    RHIDeviceContext* GetImmediateContext();
    ShaderProgram* CreateShaderFromHlslFile(const char *fileName);
    ComputeShader* CreateComputeShaderFromHlslFile(const char *fileName);
    VertexBuffer* CreateVertexBuffer(Vertex3 *vertices, unsigned int vertexCount);
    IndexBuffer* CreateIndexBuffer(unsigned int const *indices, unsigned int const index_count, eBufferUsage const usage = BUFFERUSAGE_STATIC);
    static ID3DBlob* CompileHLSLToShaderBlob(char const *opt_filename, void const *source_code, size_t const source_code_size, char const *entrypoint, char const* target);
    ID3DBlob* CompileHLSLToComputeShaderBlob(char const *opt_filename, void const *source_code, size_t const source_code_size, char const *entrypoint, char const* target);
        void* FileReadToBuffer(char const *filename, size_t *out_size);

public:
    RHIDeviceContext *m_immediateContext;
    ID3D11Device *m_dxDevice;
};
