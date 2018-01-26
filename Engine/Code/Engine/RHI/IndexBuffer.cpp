#include "Engine/RHI/IndexBuffer.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
IndexBuffer::IndexBuffer(RHIDevice *owner,
    unsigned int const *indices,
    unsigned int const indexCount,
    eBufferUsage usage)
{
    m_usage = usage;
    m_dxBuffer = nullptr;
    m_bufferSize = 0U;

    Create(owner, indices, indexCount);
}

//*******************************************************************
IndexBuffer::~IndexBuffer()
{
	//clean up with delete and set to nullptr
    if (m_dxBuffer != nullptr)
    {
        m_dxBuffer->Release();
        m_dxBuffer = nullptr;
    }	
}

//*******************************************************************
void IndexBuffer::Update(RHIDeviceContext *context, unsigned int const *indices, unsigned int const indexCount)
{
    size_t size = sizeof(unsigned int) * indexCount;

    bool succeed = false;
    if (size > m_bufferSize) {
        // too large - can't map it.  Recreate a larger one [note, will require a rebind
        // to the context before it takes affect]
        Create(context->m_device, indices, indexCount);
        succeed = true;
    }
    else 
    {
        // Smaller than the block of memory allocated - so just copy into it.
        D3D11_MAPPED_SUBRESOURCE resource;
        ID3D11DeviceContext *dx_context = context->m_dxContext;
        if (SUCCEEDED(dx_context->Map(m_dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0U, &resource))) 
        {
            memcpy(resource.pData, indices, size);
            dx_context->Unmap(m_dxBuffer, 0);
            succeed = true;
        }
    }

    ASSERT_OR_DIE(succeed, "Did not map index buffer properly\n");
}

//*******************************************************************
bool IndexBuffer::Create(RHIDevice *device, unsigned int
    const *data, unsigned int const count)
{
    if (m_dxBuffer != nullptr)
    {
        m_dxBuffer->Release();
        m_dxBuffer = nullptr;
    }    

    m_bufferSize = count * sizeof(unsigned int);

    D3D11_BUFFER_DESC ib_desc;
    MemZero(&ib_desc);

    ib_desc.ByteWidth = m_bufferSize;
    ib_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;//D3D11_BIND_VERTEX_BUFFER;
    ib_desc.StructureByteStride = sizeof(unsigned int);

    if (m_usage == BUFFERUSAGE_DYNAMIC) {
        ib_desc.Usage = D3D11_USAGE_DYNAMIC;
        ib_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else {
        ib_desc.Usage = D3D11_USAGE_IMMUTABLE;
    }


    m_dxBuffer = nullptr;
    if (nullptr != data) {
        D3D11_SUBRESOURCE_DATA initial_data;
        memset(&initial_data, 0, sizeof(initial_data));
        initial_data.pSysMem = data;

        device->m_dxDevice->CreateBuffer(&ib_desc, &initial_data, &m_dxBuffer);
    }
    else {
        device->m_dxDevice->CreateBuffer(&ib_desc, nullptr, &m_dxBuffer);
    }

    if (m_dxBuffer == nullptr) {
        m_bufferSize = 0U;
        return false;
    }
    else {
        return true;
    }
}
