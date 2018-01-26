#pragma once
#include "Engine/RHI/DX11.hpp"
#include "Engine/Math/Vertex3.hpp"

class RHIDevice;
class RHIDeviceContext;

class StructuredBuffer
{
public:
	StructuredBuffer(RHIDevice *owner,
		void const *data,
        unsigned int object_size,
        unsigned int object_count);
	~StructuredBuffer();

    bool CreateViews(RHIDevice *device);
    bool Update(RHIDeviceContext *context, void const *buffer);
    inline bool isValid() const { return (m_dxBuffer != nullptr); }


public:
	ID3D11Buffer *m_dxBuffer;
    ID3D11ShaderResourceView *m_dxSrv;

    unsigned int m_objCount;
    unsigned int m_objSize;
    size_t m_bufferSize;
};