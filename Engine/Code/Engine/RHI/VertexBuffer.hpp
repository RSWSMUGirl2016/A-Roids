#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/Math/Vertex3.hpp"
#include "Engine/RHI/RHIDeviceContext.hpp"

class RHIDevice;

class VertexBuffer
{
public:
	VertexBuffer(RHIDevice *owner,
		Vertex3 const *vertices,
        unsigned int const vertexCount,
        eBufferUsage usage = BUFFERUSAGE_STATIC);
	~VertexBuffer();
    void Update(RHIDeviceContext *context, Vertex3 const *vertices, unsigned int const vertexCount);
    inline bool isValid() const { return (m_dxBuffer != nullptr); }

    bool Create(RHIDevice *device, Vertex3
        const *data, unsigned int const count);

public:
	ID3D11Buffer *m_dxBuffer;
    unsigned int m_bufferSize;
    eBufferUsage m_usage;
};