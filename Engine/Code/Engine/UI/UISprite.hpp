#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/RHI/Texture2D.hpp"

class UISprite : public UIElement
{
public:
    Rgba m_tint;
    AABB2D m_uvCoordinates;
    Texture2D* m_texture;
public:
    UISprite();
    ~UISprite();

    void SetTint(Rgba tint);
    void SetUvCoordinates(AABB2D uvCoordinates);
    void SetTexture(Texture2D* texture2D);

    virtual void Render() override;
};