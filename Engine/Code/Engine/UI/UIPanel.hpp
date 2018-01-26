#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/Core/Rgba.hpp"

class UIPanel : public UIElement
{
public:
    Rgba m_tint;
public:
    UIPanel();
    ~UIPanel();

    void SetTint(Rgba tint);

    virtual void Render() override;
};