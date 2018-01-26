#pragma once

#include "Engine/RHI/RHITypes.hpp"

#include <d3d11.h>
#include <d3d10.h>
#include <DXGI.h>

#include <dxgidebug.h>

#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3d10.lib" )
#pragma comment( lib, "DXGI.lib" )

#include <d3dcompiler.h>
#pragma comment( lib, "d3dcompiler.lib" )

D3D11_USAGE DXGetBufferUsage( eBufferUsage const usage );
D3D11_PRIMITIVE_TOPOLOGY DXGetTopology( ePrimitiveType const type );