#pragma once
#include "Engine/UI/UIElement.hpp"
#include "Engine/RHI/Texture2D.hpp"

class UICanvas : public UIElement
{
public:
    float m_referenceResolution;
    Texture2D* m_renderTarget;

public:
    UICanvas();
    ~UICanvas();

    void SetReferenceResolution(float resolution);
    void SetRenderTarget(Texture2D* renderTarget);

    virtual void Render() override;
};
