#include "Engine/UI/UICanvas.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Engine/EngineCommon.hpp"

//*******************************************************************
UICanvas::UICanvas()
    : m_referenceResolution(0.0f)
    , m_renderTarget(nullptr)
{
}

//*******************************************************************
UICanvas::~UICanvas()
{
    if (m_renderTarget != nullptr)
    {
        delete m_renderTarget;
        m_renderTarget = nullptr;
    }
}

//*******************************************************************
void UICanvas::SetReferenceResolution(float resolution)
{
    m_referenceResolution = resolution;
}

//*******************************************************************
void UICanvas::SetRenderTarget(Texture2D* renderTarget)
{
    m_renderTarget = renderTarget;
        
    float aspectRatio = ORHTO_WIDTH / ORHTO_HEIGHT;
    if (1.0f > aspectRatio)
    {
        ORHTO_WIDTH = (1.0f / aspectRatio) * m_referenceResolution;
        ORHTO_HEIGHT = m_referenceResolution;
    }
    else
    {
        ORHTO_WIDTH = m_referenceResolution;
        ORHTO_HEIGHT = (1.0f / aspectRatio) * m_referenceResolution;
    }

    SetPosition(Vector2(0.0f, 0.0f), Vector2());
    SetPivot(Vector2());
    SetSize(Vector2(ORHTO_WIDTH, ORHTO_HEIGHT));
    SetSizeRatio(Vector2(1.0f, 1.0f));
}

//*******************************************************************
void UICanvas::Render()
{
    RenderChildElements();
}
