#pragma once

unsigned int constexpr MAX_COUNT = (unsigned int)(-1);
float constexpr MAX_FLOAT = (3.402823466e+38f); // stolen from D3D11

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);

enum eBufferUsage : unsigned int
{
    BUFFERUSAGE_GPU,     // GPU Read/Write, CPU Full Writes        (RenderTargets?)
    BUFFERUSAGE_STATIC,  // GPU Read, Written only at creation     (TEXTURES, STATIC MESHES)
    BUFFERUSAGE_DYNAMIC, // GPU Read, CPU Write (Partial or Full)  (DYNAMIC MESHES/DYNAMIC TEXTURES)
    BUFFERUSAGE_STAGING, // GPU Read/Write, CPU Read/Write         Slowest - image composition/manipulation
};

//*******************************************************************
enum class ePrimitiveType : unsigned int
{
    PRIMITIVE_NONE,
    PRIMITIVE_POINTS,
    PRIMITIVE_LINES,
    PRIMITIVE_LINE_STRIP,
    PRIMITIVE_TRIANGLES,
    PRIMITIVE_TRIANGLE_STRIP,
};

//*******************************************************************
enum eWrapMode : unsigned __int8
{
    WRAPMODE_WRAP,
    WRAPMODE_CLAMP,
    WRAPMODE_MIRROR,
    WRAPMODE_BORDER,
};

//*******************************************************************
enum eFilterMode : unsigned __int8
{
    FILTER_POINT,
    FILTER_LINEAR,
    FILTERMODE_NEAREST,
    FILTERMODE_LINEAR,
    FILTERMODE_NONE,        
    FILTERMODE_ANISOTROPIC,
};

//*******************************************************************
template <typename T>
inline int Compare(T const &a, T const &b)
{
    return a - b;
}

//*******************************************************************
template <typename T>
inline void MemZero(T *ptr)
{
    memset(ptr, 0, sizeof(T));
}