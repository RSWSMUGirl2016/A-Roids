#pragma once
#include "Engine/Core/Rgba.hpp"

enum eImageFormat : unsigned int
{
    IMAGEFORMAT_RGBA8,
    IMAGEFORMAT_D24S8,
};

class Image
{
public:
    Image()
        : m_buffer(nullptr)
        , m_width(0)
        , m_height(0)
        , m_bpp(0)
    {}
    
    ~Image();

    bool LoadFromFile(char const *filename);
    bool CreateClear(unsigned int width, unsigned int height, Rgba color);
    void Destroy();

    inline bool IsValid() const { return nullptr != m_buffer; }

    inline unsigned int GetPixel_size() const { return m_bpp; }
    inline unsigned int GetWidth() const { return m_width; }
    inline unsigned int GetHeight() const { return m_height; }
    inline unsigned int GetStride() const { return m_width * m_bpp; }
    inline unsigned int GetSlice() const { return GetStride() * m_height; }

public:
    unsigned __int8 *m_buffer;
    unsigned int m_width;
    unsigned int m_height;
    unsigned m_bpp;

    eImageFormat m_format;
};