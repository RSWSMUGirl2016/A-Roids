#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Math/Vertex3.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/RHI/ShaderProgram.hpp"
#include "Engine/RHI/VertexBuffer.hpp"
#include "Engine/RHI/IndexBuffer.hpp"
#include "Engine/RHI/ComputeShader.hpp"
#include "Engine/EngineCommon.hpp"

//*******************************************************************
RHIDevice::RHIDevice()
	:m_immediateContext(nullptr),
	m_dxDevice(nullptr)
{
}

//*******************************************************************
RHIDevice::~RHIDevice()
{
    /*if (m_immediateContext != nullptr)
    {
        delete m_immediateContext;
        m_immediateContext = nullptr;
    }*/
	
    if (m_dxDevice != nullptr)
    {
        m_dxDevice->Release();
        m_dxDevice = nullptr;
    }
}

//*******************************************************************
Texture2D* RHIDevice::CreateTexture2DFromFile(char const *filename)
{
    return new Texture2D(this, filename);
}

//*******************************************************************
Sampler* RHIDevice::CreateSampler(sampler_desc_t const *optDesc)
{
    return new Sampler(this, optDesc);
}

//*******************************************************************
RHIDeviceContext* RHIDevice::GetImmediateContext()
{	
	return m_immediateContext;
}

//*******************************************************************
ShaderProgram* RHIDevice::CreateShaderFromHlslFile(const char *fileName)
{
	size_t src_size = 0U;
	void *src_code = FileReadToBuffer(fileName, &src_size);
	if (src_code == nullptr)
	{
		return nullptr;
	}

	ID3DBlob *vs_byte_code = nullptr;
	ID3DBlob *fs_byte_code = nullptr;

	vs_byte_code = CompileHLSLToShaderBlob(fileName, src_code, src_size, "VertexFunction", "vs_5_0");
	fs_byte_code = CompileHLSLToShaderBlob(fileName, src_code, src_size, "FragmentFunction", "ps_5_0");
	delete src_code;

	if ((vs_byte_code != nullptr) && (fs_byte_code != nullptr))
	{
		ID3D11VertexShader *vs;
		ID3D11PixelShader *fs;

		m_dxDevice->CreateVertexShader(vs_byte_code->GetBufferPointer(), vs_byte_code->GetBufferSize(), nullptr, &vs);
		m_dxDevice->CreatePixelShader(fs_byte_code->GetBufferPointer(), fs_byte_code->GetBufferSize(), nullptr, &fs);

		ShaderProgram *program = new ShaderProgram(this, vs, fs, vs_byte_code, fs_byte_code);
		return program;
	}

	return nullptr;
}

//*******************************************************************
ComputeShader* RHIDevice::CreateComputeShaderFromHlslFile(const char *fileName)
{
    size_t src_size = 0U;
    void *src_code = FileReadToBuffer(fileName, &src_size);
    if (src_code == nullptr)
    {
        return nullptr;
    }

    ID3DBlob *cs_byte_code = nullptr;
    cs_byte_code = CompileHLSLToComputeShaderBlob(fileName, src_code, src_size, "Main", "cs_5_0");

    if (cs_byte_code != nullptr)
    {
        ID3D11ComputeShader *cs = nullptr;

        HRESULT hr = m_dxDevice->CreateComputeShader(cs_byte_code->GetBufferPointer(), cs_byte_code->GetBufferSize(), nullptr, &cs);

        if (FAILED(hr))
        {
            ASSERT_OR_DIE(false, "Did not create a compute shader!");
        }

        ComputeShader *shader = new ComputeShader(this, cs, cs_byte_code);
        return shader;
    }

    return nullptr;
}

//*******************************************************************
VertexBuffer* RHIDevice::CreateVertexBuffer(Vertex3 *vertices, unsigned int vertexCount)
{
	VertexBuffer *buffer = new VertexBuffer(this, vertices, vertexCount);
	return buffer;
}

//*******************************************************************
IndexBuffer* RHIDevice::CreateIndexBuffer(unsigned int const *indices, unsigned int const index_count, eBufferUsage const usage /*= BUFFERUSAGE_STATIC*/)
{
    UNUSED(usage);
    IndexBuffer *buffer = new IndexBuffer(this, indices, index_count);
    return buffer;
}

//*******************************************************************
#ifdef _DEBUG
#define DEBUG_SHADERS
#endif // _DEBUG

//*******************************************************************
ID3DBlob* RHIDevice::CompileHLSLToShaderBlob(char const *opt_filename,  // optional: used for error messages
	void const *source_code,                                          // buffer containing source code.
	size_t const source_code_size,                                    // size of the above buffer.
	char const *entrypoint,                                           // Name of the Function we treat as the entry point for this stage
	char const* target)                                             // What stage we're compiling for (Vertex/Fragment/Hull/Compute... etc...)
{
	/* DEFINE MACROS - CONTROLS SHADER
	// You can optionally define macros to control compilation (for instance, DEBUG builds, disabling lighting, etc...)
	D3D_SHADER_MACRO defines[1];
	defines[0].Name = "TEST_MACRO";
	defines[0].Definition = nullptr;
	*/

	DWORD compile_flags = 0U;
	#if defined(DEBUG_SHADERS)
		compile_flags |= D3DCOMPILE_DEBUG;
		compile_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
		compile_flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;   // cause, FIX YOUR WARNINGS
	#else 
		// compile_flags |= D3DCOMPILE_SKIP_VALIDATION;       // Only do this if you know for a fact this shader works with this device (so second run through of a game)
		compile_flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;   // Yay, fastness (default is level 1)
	#endif

	ID3DBlob *code = nullptr;
	ID3DBlob *errors = nullptr;

	HRESULT hr = ::D3DCompile(source_code,
		source_code_size,                   // plain text source code
		opt_filename,                       // optional, used for error messages (If you HLSL has includes - it will not use the includes names, it will use this name)
		nullptr,                            // pre-compiler defines - used more for compiling multiple versions of a single shader (different quality specs, or shaders that are mostly the same outside some constants)
		D3D_COMPILE_STANDARD_FILE_INCLUDE,  // include rules - this allows #includes in the shader to work relative to the src_file path or my current working directly
		entrypoint,                         // Entry Point for this shader
		target,                             // Compile Target (MSDN - "Specifying Compiler Targets")
		compile_flags,                      // Flags that control compilation
		0,                                  // Effect Flags (we will not be doing Effect Files)
		&code,                              // [OUT] ID3DBlob (buffer) that will store the byte code.
		&errors);                          // [OUT] ID3DBlob (buffer) that will store error information

	if (FAILED(hr) || (errors != nullptr)) {
		if (errors != nullptr) {
			char *error_string = (char*)errors->GetBufferPointer();
			DebuggerPrintf("Failed to compile [%s].  Compiler gave the following output;\n%s",
				opt_filename,
				error_string);
			errors->Release();
			errors = nullptr;
		}
	}


	// will be nullptr if it failed to compile
	return code;
}

//*******************************************************************
ID3DBlob* RHIDevice::CompileHLSLToComputeShaderBlob(char const *opt_filename, void const *source_code, size_t const source_code_size, char const *entrypoint, char const* target) 
{
    UNUSED(target);
    UNUSED(source_code_size);
    UNUSED(source_code);

    DWORD compile_flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG_SHADERS)
    compile_flags |= D3DCOMPILE_DEBUG;
    compile_flags |= D3DCOMPILE_SKIP_OPTIMIZATION;
    compile_flags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else 
    compile_flags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    LPCSTR profile = "cs_5_0";
    ID3DBlob *code = nullptr;
    ID3DBlob *errors = nullptr;

    WCHAR fileName[1024];
    MultiByteToWideChar(GetACP(), 0, opt_filename, -1, fileName, sizeof(fileName) / sizeof(fileName[0]));
    HRESULT hr = ::D3DCompileFromFile(fileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint, profile, compile_flags, 0, &code, &errors);

    if (FAILED(hr) || (errors != nullptr)) {
        if (errors != nullptr) {
            char *error_string = (char*)errors->GetBufferPointer();
            DebuggerPrintf("Failed to compile [%s].  Compiler gave the following output;\n%s",
                opt_filename,
                error_string);
            errors->Release();
            errors = nullptr;
        }
    }

    return code;
}

//*******************************************************************
void* RHIDevice::FileReadToBuffer(char const *filename, size_t *out_size)
{
	*out_size = 0U;

	// I'm going to use Windows file functions for no particular reason
	// outside of familiarity.  Feel free to use whatever. 
	DWORD file_access = GENERIC_READ;
	DWORD share = FILE_SHARE_READ;
	DWORD options = OPEN_EXISTING;

	HANDLE file_handle = CreateFileA(filename, file_access, share, NULL, options, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		//Logf("File [%s] could not be opened.", filename);
		return nullptr;
	}

	// Get File Size
	DWORD high_word;
	DWORD lo_word = ::GetFileSize(file_handle, &high_word);

	// 64-bit way - use a 64-bit type, but we will never have a 4GB+ file, so ignore it and just using lo-word to prevent warnings
	// size_t size = ((size_t)high_word << 32) | (size_t)lo_word;
	size_t size = (size_t)lo_word;

	// Make a buffer that big
	void *buffer = malloc(size + 1U); // I allocae one extra for a null terminator - but do not include it in the size
									  // just in the cases I load strings it null terminates.
	if (nullptr != buffer) {
		DWORD read = 0U;
		ReadFile(file_handle, buffer, (DWORD)size, &read, nullptr);
		*out_size = read;

		((char*)buffer)[size] = NULL;
	}

	CloseHandle((HANDLE)file_handle);
	return buffer;
}

