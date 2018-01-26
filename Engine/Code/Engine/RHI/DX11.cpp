#include "Engine/RHI/DX11.hpp"

D3D11_USAGE DXGetBufferUsage(eBufferUsage const usage)
{
    return (D3D11_USAGE)usage;
}

D3D11_PRIMITIVE_TOPOLOGY DXGetTopology(ePrimitiveType const type)
{
    return (D3D11_PRIMITIVE_TOPOLOGY)type;
}
