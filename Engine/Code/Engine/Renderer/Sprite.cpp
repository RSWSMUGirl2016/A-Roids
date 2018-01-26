#include "Engine/Renderer/Sprite.hpp"

//*******************************************************************
Sprite::Sprite(Texture* texture, AABB2D uvCoordinates, Vector2 pivot)
    : m_spriteTexture(texture)
    , m_spriteTexture2D(nullptr)
    , m_uvCoordinates(uvCoordinates)
    , m_pivot(pivot)
{
}

//*******************************************************************
Sprite::Sprite(Texture2D* texture, AABB2D uvCoordinates, Vector2 pivot)
    : m_spriteTexture(nullptr)
    , m_spriteTexture2D(texture)
    , m_uvCoordinates(uvCoordinates)
    , m_pivot(pivot)
{
}

//*******************************************************************
Texture* Sprite::GetTexture()
{
    return m_spriteTexture;
}

//*******************************************************************
Texture* Sprite::GetTexture() const
{
    return m_spriteTexture;
}

//*******************************************************************
Texture2D* Sprite::GetTexture2D()
{
    return m_spriteTexture2D;
}

//*******************************************************************
Texture2D* Sprite::GetTexture2D() const
{
    return m_spriteTexture2D;
}

//*******************************************************************
AABB2D Sprite::GetUVCoordinates()
{
    return m_uvCoordinates;
}

//*******************************************************************
Vector2 Sprite::GetPivot()
{
    return m_pivot;
}

//*******************************************************************
float Sprite::GetTexelWidth()
{
    return 0; 
}

//*******************************************************************
float Sprite::GetTexelHeight()
{
    return 0;
}

//*******************************************************************
float Sprite::GetPixelsPerUnit()
{
    return 0;
}
