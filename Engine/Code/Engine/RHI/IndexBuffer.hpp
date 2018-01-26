#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/Math/Vertex3.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"

class RHIDevice;

class IndexBuffer
{
public:
	IndexBuffer(RHIDevice *owner,
        unsigned int const *indices,
        unsigned int const indexCount,
        eBufferUsage usage = BUFFERUSAGE_STATIC);
	~IndexBuffer();
    void Update(RHIDeviceContext *context, unsigned int const *indices, unsigned int const indexCount);
    inline bool isValid() const { return (m_dxBuffer != nullptr); }

    bool Create(RHIDevice *device, unsigned int
        const *data, unsigned int const count);

public:
	ID3D11Buffer *m_dxBuffer;
    unsigned int m_bufferSize;
    eBufferUsage m_usage;
};