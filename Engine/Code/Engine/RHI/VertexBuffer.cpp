#include "Engine/RHI/VertexBuffer.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
VertexBuffer::VertexBuffer(RHIDevice *device, Vertex3 const *vertices, unsigned int const vertexCount,
    eBufferUsage usage)
{
    m_usage = usage;
    m_dxBuffer = nullptr;
    m_bufferSize = 0U;

    Create(device, vertices, vertexCount);
}

//*******************************************************************
VertexBuffer::~VertexBuffer()
{
	//clean up with delete and set to nullptr
    if (m_dxBuffer != nullptr)
    {
        m_dxBuffer->Release();
        m_dxBuffer = nullptr;
    }	
}

//*******************************************************************
void VertexBuffer::Update(RHIDeviceContext *context, Vertex3 const *vertices, unsigned int const vertexCount)
{
    size_t size = sizeof(Vertex3) * vertexCount;

    bool succeed = false;
    if (size > m_bufferSize) {
        // too large - can't map it.  Recreate a larger one [note, will require a rebind
        // to the context before it takes affect]
        Create(context->m_device, vertices, vertexCount);
        succeed = true;
    }
    else 
    {
        // Smaller than the block of memory allocated - so just copy into it.
        D3D11_MAPPED_SUBRESOURCE resource;
        ID3D11DeviceContext *dx_context = context->m_dxContext;
        if (SUCCEEDED(dx_context->Map(m_dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0U, &resource))) 
        {
            memcpy(resource.pData, vertices, size);
            dx_context->Unmap(m_dxBuffer, 0);
            succeed = true;
        }
    }

    ASSERT_OR_DIE(succeed, "Did not map vertex buffer properly\n");
}

//*******************************************************************
bool VertexBuffer::Create(RHIDevice *device, Vertex3 const *data, unsigned int const count)
{
    if (m_dxBuffer != nullptr)
    {
        m_dxBuffer->Release();
        m_dxBuffer = nullptr;
    }    

    m_bufferSize = count * sizeof(Vertex3);

    D3D11_BUFFER_DESC vb_desc;
    MemZero(&vb_desc);

    vb_desc.ByteWidth = m_bufferSize;
    vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vb_desc.StructureByteStride = sizeof(Vertex3);

    if (m_usage == BUFFERUSAGE_DYNAMIC) {
        vb_desc.Usage = D3D11_USAGE_DYNAMIC;
        vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else {
        vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
    }


    m_dxBuffer = nullptr;
    if (nullptr != data) {
        D3D11_SUBRESOURCE_DATA initial_data;
        memset(&initial_data, 0, sizeof(initial_data));
        initial_data.pSysMem = data;

        device->m_dxDevice->CreateBuffer(&vb_desc, &initial_data, &m_dxBuffer);
    }
    else {
        device->m_dxDevice->CreateBuffer(&vb_desc, nullptr, &m_dxBuffer);
    }

    if (m_dxBuffer == nullptr) {
        m_bufferSize = 0U;
        return false;
    }
    else {
        return true;
    }
}
