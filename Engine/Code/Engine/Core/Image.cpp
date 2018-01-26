#include "Image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_PSD
#define STBI_NO_PNM
#include "ThirdParty/stb/stb_image.h"

//*******************************************************************
Image::~Image()
{
    Destroy();
}

//*******************************************************************
bool Image::LoadFromFile(char const *filename)
{
    Destroy();

    int width;
    int height;
    int comp;

    stbi_uc *data = stbi_load(filename, &width, &height, &comp, 0);
    if (data != nullptr)
    {
        if (comp == 3)
        {
            stbi_uc *newBuffer = stbi__convert_format(data, comp, 4, width, height);
            data = newBuffer;
        }

        m_buffer = (unsigned __int8*)data;
        m_width = width;
        m_height = height;
        m_bpp = 4;
        m_format = IMAGEFORMAT_RGBA8;
        return true;
    }
    return false;
}

//*******************************************************************
bool Image::CreateClear(unsigned int width, unsigned int height, Rgba color)
{
    Destroy();

    m_width = width;
    m_height = height;
    m_format = IMAGEFORMAT_RGBA8;
    m_bpp = 4;

    unsigned int size = m_width * m_height * m_bpp;
    m_buffer = (unsigned char*) ::malloc(size);

    float r;
    float g;
    float b;
    float a;
    color.GetAsFloats(r, g, b, a);

    rgba_u32 c = rgba_u32(r, g, b, a);

    rgba_u32 *colors = (rgba_u32*)m_buffer;
    unsigned int count = m_width * m_height;
    for (unsigned int i = 0; i < count; ++i)
    {
        colors[i] = c;
    }

    return true;
}

//*******************************************************************
void Image::Destroy()
{
    if (nullptr != m_buffer)
    {
        ::free(m_buffer);
    }
}