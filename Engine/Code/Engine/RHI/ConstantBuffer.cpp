#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

ConstantBuffer::ConstantBuffer(RHIDevice *owner,
    void const *buffer,
    size_t const bufferSize)
{
    m_bufferSize = bufferSize;

	D3D11_BUFFER_DESC vb_desc;
	memset(&vb_desc, 0, sizeof(vb_desc));

	vb_desc.ByteWidth = (unsigned int)bufferSize;  // How much data are we putting into this buffer
	vb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;         // What can we bind this data as (in this case, only vertex data)
	vb_desc.Usage = D3D11_USAGE_DYNAMIC;                // Hint on how this memory is used (in this case, it is immutable, or constant - can't be changed)
														  // for limitations/strenghts of each, see;  
	
    vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;                                                     //    https://msdn.microsoft.com/en-us/library/windows/desktop/ff476259(v=vs.85).aspx
	vb_desc.StructureByteStride = 0;       // How large is each element in this buffer

														  // Next, setup the initial data (required since this is an immutable buffer - so it must be instantiated at creation time)
	D3D11_SUBRESOURCE_DATA initial_data;
	memset(&initial_data, 0, sizeof(initial_data));
	initial_data.pSysMem = buffer;

	// Finally create the vertex buffer
	m_dxBuffer = nullptr;
    owner->m_dxDevice->CreateBuffer(&vb_desc, &initial_data, &m_dxBuffer);

    if (m_dxBuffer == nullptr)
    {
        ASSERT_OR_DIE(false, "ConstantBuffer's m_dxBuffer is nullptr");
    }
}

ConstantBuffer::~ConstantBuffer()
{
	//clean up with delete and set to nullptr
    if (m_dxBuffer != nullptr)
    {
        m_dxBuffer->Release();
        m_dxBuffer = nullptr;
    }
}


bool ConstantBuffer::Update(RHIDeviceContext *context, void const *buffer)
{
    D3D11_MAPPED_SUBRESOURCE resource;

    ID3D11DeviceContext *dxContext = context->m_dxContext;
    if (SUCCEEDED(dxContext->Map(m_dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0U, &resource)))
    {
        memcpy(resource.pData, buffer, m_bufferSize);
        dxContext->Unmap(m_dxBuffer, 0);

        return true;
    }

    //assert or die
    return false;
}
