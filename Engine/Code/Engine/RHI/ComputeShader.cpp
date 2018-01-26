#include "Engine/RHI/ComputeShader.hpp"

//*******************************************************************
ComputeShader::ComputeShader(RHIDevice *owner, char const *filename)
{
    owner->CreateComputeShaderFromHlslFile(filename);
}

//*******************************************************************
ComputeShader::~ComputeShader()
{
    Destroy();
}

//*******************************************************************
void ComputeShader::Destroy()
{
    if (m_dxComputeShader != nullptr)
    {
        m_dxComputeShader->Release();
        m_dxComputeShader = nullptr;
    }
}
