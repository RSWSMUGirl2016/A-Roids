#include "Engine/UI/UIPanel.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
UIPanel::UIPanel()
    : m_tint(Rgba::TRANSPRNT)
{
}

//*******************************************************************
UIPanel::~UIPanel()
{
}

//*******************************************************************
void UIPanel::SetTint(Rgba tint)
{
    m_tint = tint;
}

//*******************************************************************
void UIPanel::Render()
{
    AABB2D uiElementBounds = GetElementBounds();
    Matrix4 worldTransform = GetWorldTransform();
    Vector2 transformedMins = MultiplyVector2ByMatrix4(uiElementBounds.m_mins, worldTransform);
    Vector2 transformedMaxs = MultiplyVector2ByMatrix4(uiElementBounds.m_maxs, worldTransform);
    g_theSimpleRenderer->SetTexture(g_theSimpleRenderer->m_whiteSample);
    g_theSimpleRenderer->DrawQuad(transformedMins, transformedMaxs, Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f), m_tint);
}
