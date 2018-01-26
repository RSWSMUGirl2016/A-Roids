#pragma once
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/RHI/Texture2D.hpp"

class Sprite
{
public:
    Sprite(Texture* texture, AABB2D uvCoordinates, Vector2 pivot);
    Sprite(Texture2D* texture, AABB2D uvCoordinates, Vector2 pivot);

    Texture* GetTexture();
    Texture2D* GetTexture2D();
    Texture* GetTexture() const;
    Texture2D* GetTexture2D() const;
    AABB2D GetUVCoordinates();
    Vector2 GetPivot();
    float GetTexelWidth();
    float GetTexelHeight();
    float GetPixelsPerUnit();

private:
    Texture* m_spriteTexture;
    Texture2D* m_spriteTexture2D;
    AABB2D m_uvCoordinates;
    Vector2 m_pivot;
};