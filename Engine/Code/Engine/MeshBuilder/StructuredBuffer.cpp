#include "Engine/MeshBuilder/StructuredBuffer.hpp"
#include "Engine/RHI/RHIDevice.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
StructuredBuffer::StructuredBuffer(RHIDevice *owner, void const *data, unsigned int object_size, unsigned int object_count)
{
    m_dxSrv = nullptr;
    m_bufferSize = object_size * object_count;
    m_objCount = object_count;
    m_objSize = object_size;

    // First, describe the buffer
    D3D11_BUFFER_DESC vb_desc;
    memset(&vb_desc, 0, sizeof(vb_desc));

    vb_desc.ByteWidth = (unsigned int)m_bufferSize;                // How much data are we putting into this buffer
    vb_desc.Usage = D3D11_USAGE_DYNAMIC;                  // Hint on how this memory is used (in this case, it is immutable, or constant - can't be changed)
                                                          // for limitations/strenghts of each, see;  
                                                          //    https://msdn.microsoft.com/en-us/library/windows/desktop/ff476259(v=vs.85).aspx
    vb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    // CHANGES FROM CONSTANT BUFFER!!!
    vb_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;       // What can we bind this data as (in this case, only vertex data)
    vb_desc.StructureByteStride = object_size;            // How large is each element in this buffer
    vb_desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

    // Next, setup the initial data (required since this is an immutable buffer - so it must be instantiated at creation time)
    D3D11_SUBRESOURCE_DATA initial_data;
    memset(&initial_data, 0, sizeof(initial_data));
    initial_data.pSysMem = data;

    // Finally create the vertex buffer
    m_dxBuffer = nullptr;
    owner->m_dxDevice->CreateBuffer(&vb_desc, &initial_data, &m_dxBuffer);
    CreateViews(owner);
}

//*******************************************************************
StructuredBuffer::~StructuredBuffer()
{
    if (m_dxBuffer != nullptr)
    {
        m_dxBuffer->Release();
        m_dxBuffer = nullptr;
    }
    if (m_dxSrv != nullptr)
    {
        m_dxSrv->Release();
        m_dxSrv = nullptr;
    }
}

//*******************************************************************
bool StructuredBuffer::CreateViews(RHIDevice *device)
{
    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    MemZero(&desc);

    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    desc.Buffer.ElementOffset = 0U;
    desc.Buffer.NumElements = m_objCount;

    HRESULT result = device->m_dxDevice->CreateShaderResourceView(m_dxBuffer, &desc, &m_dxSrv);
    //ASSERT_RETURN_VALUE(SUCCEEDED(result), false);
    if (FAILED(result))
    {
        ASSERT_OR_DIE(false, "Creating Shader Resource View failed.");
    }

    return true;
}

//*******************************************************************
bool StructuredBuffer::Update(RHIDeviceContext *context, void const *buffer)
{
    D3D11_MAPPED_SUBRESOURCE resource;

    ID3D11DeviceContext *dx_context = context->m_dxContext;
    if (SUCCEEDED(dx_context->Map(m_dxBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0U, &resource))) {
        memcpy(resource.pData, buffer, m_bufferSize);
        dx_context->Unmap(m_dxBuffer, 0);

        return true;
    }

    // ASSERT?
    ASSERT_OR_DIE(false, "StructureBuffer not updating properly!");
    return false;
}
