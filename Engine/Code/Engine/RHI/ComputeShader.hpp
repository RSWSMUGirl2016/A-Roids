#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/EngineCommon.hpp"

//	User the shader target: cs_5_0
//  Shader is created using ID3D11Device::CreateComputeShader
//	    Shader stored in an ID3D11ComputeShader object.


class ComputeShader
{
public:
    ComputeShader(RHIDevice *owner, char const *filename);
    ComputeShader(RHIDevice *device, ID3D11ComputeShader *cs, ID3DBlob *cs_bytecode)
        : m_dxComputeShader(cs)
        , m_csByteCode(cs_bytecode)
    {
        UNUSED(device);
    };
    ~ComputeShader();
    void Destroy();

public:
    ID3D11ComputeShader *m_dxComputeShader;
    ID3DBlob *m_csByteCode;
};