#include "Engine/UI/UISprite.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
UISprite::UISprite()
    : m_tint(Rgba::WHITE)
    , m_uvCoordinates(AABB2D(Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f)))
    , m_texture(nullptr)
{
}

//*******************************************************************
UISprite::~UISprite()
{
}

//*******************************************************************
void UISprite::SetTint(Rgba tint)
{
    m_tint = tint;
}

//*******************************************************************
void UISprite::SetUvCoordinates(AABB2D uvCoordinates)
{
    m_uvCoordinates = uvCoordinates;
}

//*******************************************************************
void UISprite::SetTexture(Texture2D* texture2D)
{
    m_texture = texture2D;
}

//*******************************************************************
void UISprite::Render()
{
    AABB2D uiElementBounds = GetElementBounds();
    Matrix4 worldTransform = GetWorldTransform();
    Vector2 transformedMins = MultiplyVector2ByMatrix4(uiElementBounds.m_mins, worldTransform);
    Vector2 transformedMaxs = MultiplyVector2ByMatrix4(uiElementBounds.m_maxs, worldTransform);
    g_theSimpleRenderer->SetTexture(m_texture);
    g_theSimpleRenderer->DrawQuad(transformedMins, transformedMaxs, m_uvCoordinates.m_mins, m_uvCoordinates.m_maxs, m_tint);
}
