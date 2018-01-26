#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/RHI/RHIDevice.hpp"

enum eShaderStage
{
	SHADER_VERTEX,    // maps to target "vs_5_0", or Vertex Shader Model 5 (currently latest)
	SHADER_FRAGMENT,  // maps to target "ps_5_0", or Pixel Shader Model 5 (currently latest)
};

// Technically a ShaderProgram is just the program running on the GPU (or multiple
// programs as part of a pipeline).
class ShaderProgram
{
public:
    ShaderProgram(RHIDevice *owner, char const *filename);
	ShaderProgram(RHIDevice *device, ID3D11VertexShader *vs, ID3D11PixelShader *fs, ID3DBlob *vs_bytecode, ID3DBlob *fs_bytecode)
		:m_dxVertexShader(vs)
		, m_dxFragmentShader(fs)
		, m_vsByteCode(vs_bytecode)
		, m_fsByteCode(fs_bytecode)
	{
		m_dxInputLayout = nullptr;
		CreateInputLayout(device);
	};
	~ShaderProgram();
	void CreateInputLayout(RHIDevice *rhiDevice);
    inline bool IsValid() const { return (m_dxVertexShader != nullptr) && (m_dxFragmentShader != nullptr); }

public:
	ID3D11VertexShader *m_dxVertexShader;
	ID3D11PixelShader *m_dxFragmentShader;
	ID3D11InputLayout *m_dxInputLayout;
	ID3DBlob *m_vsByteCode;
	ID3DBlob *m_fsByteCode;
};