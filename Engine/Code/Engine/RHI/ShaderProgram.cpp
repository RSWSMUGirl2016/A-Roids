#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/Math/Vertex3.hpp"

//*******************************************************************
ShaderProgram::ShaderProgram(RHIDevice *owner, char const *filename)
{
    owner->CreateShaderFromHlslFile(filename);
}

//*******************************************************************
ShaderProgram::~ShaderProgram()
{
	//DO NOTHING
    if (m_dxVertexShader != nullptr)
    {
        m_dxVertexShader->Release();
        m_dxVertexShader = nullptr;
    }

    if (m_dxFragmentShader != nullptr)
    {
        m_dxFragmentShader->Release();
        m_dxFragmentShader = nullptr;
    }

    if (m_dxInputLayout != nullptr)
    {
        m_dxInputLayout->Release();
        m_dxInputLayout = nullptr;
    }

    if (m_vsByteCode != nullptr)
    {
        m_vsByteCode->Release();
        m_vsByteCode = nullptr;
    }

    if (m_fsByteCode != nullptr)
    {
        m_fsByteCode->Release();
        m_fsByteCode = nullptr;
    }
}

//*******************************************************************
void ShaderProgram::CreateInputLayout(RHIDevice *device)
{
	D3D11_INPUT_ELEMENT_DESC desc[8];
	memset(desc, 0, sizeof(desc));

    desc[0].SemanticName = "POSITION";
    desc[0].SemanticIndex = 0;
    desc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    desc[0].InputSlot = 0U;
    desc[0].AlignedByteOffset = offsetof(Vertex3, m_position);
    desc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[0].InstanceDataStepRate = 0U;

    desc[1].SemanticName = "NORMAL";
    desc[1].SemanticIndex = 0;
    desc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    desc[1].InputSlot = 0U;
    desc[1].AlignedByteOffset = offsetof(Vertex3, m_normal);
    desc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[1].InstanceDataStepRate = 0U;

    desc[2].SemanticName = "TANGENT";
    desc[2].SemanticIndex = 0;
    desc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    desc[2].InputSlot = 0U;
    desc[2].AlignedByteOffset = offsetof(Vertex3, m_tangent);
    desc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[2].InstanceDataStepRate = 0U;

    desc[3].SemanticName = "BITANGENT";
    desc[3].SemanticIndex = 0;
    desc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    desc[3].InputSlot = 0U;
    desc[3].AlignedByteOffset = offsetof(Vertex3, m_bitangent);
    desc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[3].InstanceDataStepRate = 0U;

    desc[4].SemanticName = "TINT";
    desc[4].SemanticIndex = 0;
    desc[4].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc[4].InputSlot = 0U;
    desc[4].AlignedByteOffset = offsetof(Vertex3, m_rgba);
    desc[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[4].InstanceDataStepRate = 0U;

    desc[5].SemanticName = "UV";
    desc[5].SemanticIndex = 0;
    desc[5].Format = DXGI_FORMAT_R32G32_FLOAT;
    desc[5].InputSlot = 0U;
    desc[5].AlignedByteOffset = offsetof(Vertex3, m_uv);
    desc[5].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[5].InstanceDataStepRate = 0U;

    desc[6].SemanticName = "BONE_INDICES";
    desc[6].SemanticIndex = 0;
    desc[6].Format = DXGI_FORMAT_R32G32B32A32_UINT;
    desc[6].InputSlot = 0U;
    desc[6].AlignedByteOffset = offsetof(Vertex3, m_boneIndices);
    desc[6].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[6].InstanceDataStepRate = 0U;

    desc[7].SemanticName = "BONE_WEIGHTS";
    desc[7].SemanticIndex = 0;
    desc[7].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc[7].InputSlot = 0U;
    desc[7].AlignedByteOffset = offsetof(Vertex3, m_boneWeights);
    desc[7].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    desc[7].InstanceDataStepRate = 0U;

	device->m_dxDevice->CreateInputLayout(desc, ARRAYSIZE(desc), m_vsByteCode->GetBufferPointer(), m_vsByteCode->GetBufferSize(), &m_dxInputLayout);

}
