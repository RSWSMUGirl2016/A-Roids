#include "Engine/UI/UIElement.hpp"
#include "Engine/EngineCommon.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
UIElement::UIElement()
    : m_position(UIMetricT())
    , m_pivot(Vector2())
    , m_size(UIMetricT())
    , m_bounds(AABB2D())
    , m_parent(nullptr)
    , m_children{}
    , m_descendantsToRemove{}
{
}

//*******************************************************************
UIElement::~UIElement()
{
    if (m_parent != nullptr)
    {
        delete m_parent;
        m_parent = nullptr;
    }

    for (UIElement* child : m_children)
        delete child;
    m_children.clear();
}

//*******************************************************************
void UIElement::SetPosition(Vector2 ratio, Vector2 unit)
{
    m_position.m_ratio = ratio;
    m_position.m_unit = unit;
}

//*******************************************************************
void UIElement::SetSize(Vector2 size)
{
    m_size.m_unit = size;
}

//*******************************************************************
void UIElement::SetSizeRatio(Vector2 sizeRatio)
{
    m_size.m_ratio = sizeRatio;
}

//*******************************************************************
void UIElement::SetPivot(Vector2 ratioInRespectToMyBounds)
{
    m_pivot = ratioInRespectToMyBounds;
}

//*******************************************************************
Vector2 UIElement::GetSize()
{
    if (m_parent == nullptr)
        return m_size.m_unit;

    //Absolute Positioning
    Vector2 percentageOfParentSize = m_parent->GetSize() * m_size.m_ratio;
    return percentageOfParentSize + m_size.m_unit;
}

//*******************************************************************
AABB2D UIElement::GetElementBounds()
{
    Vector2 size = GetSize();
    
    float minX = 0.0f - m_pivot.x * size.x;
    float minY = 0.0f - m_pivot.y * size.y;
    Vector2 mins = Vector2(minX, minY);

    float maxX = (1.0f * size.x) - (m_pivot.x * size.x);
    float maxY = (1.0f * size.y) - (m_pivot.y * size.y);
    Vector2 maxs = Vector2(maxX, maxY);

    return AABB2D(mins, maxs);
}

//*******************************************************************
AABB2D UIElement::GetLocalBounds()
{
    Vector2 size = GetSize();
    Vector2 pivotPostion = size * m_pivot;

    AABB2D bounds = AABB2D(Vector2(), size);
    bounds.Translate(-1.0f * pivotPostion);
    return bounds;
}

//*******************************************************************
Vector2 UIElement::GetLocalPosition()
{
    Vector2 size = GetSize();

    if (m_parent == nullptr)
        return (size * m_pivot);

    Vector2 parentSize = m_parent->GetSize();
    return (parentSize * m_pivot) + m_position.m_unit;
}

//*******************************************************************
Matrix4 UIElement::GetLocalTransform()
{
    return Matrix4::CreateTranslation(GetLocalPosition());
}

//*******************************************************************
Matrix4 UIElement::GetWorldTransform()
{
    Matrix4 worldTransform = GetLocalTransform();

    UIElement* parent = this->m_parent;
    while (parent != nullptr)
    {
        Matrix4 parentLocalTransform = parent->GetLocalTransform();
        worldTransform = MultiplyMatrix4ByMatrix4(worldTransform, parentLocalTransform);
        parent = parent->m_parent;
    }

    return worldTransform;
}

//*******************************************************************
UIElement* UIElement::AddChild(UIElement* newChild)
{
    newChild->m_parent = this;
    m_children.push_back(newChild);
    return newChild;
}

//*******************************************************************
void UIElement::RemoveChild(UIElement* childToRemove)
{
    UIElement* descendant = childToRemove;
    while (descendant != nullptr)
    {
        m_descendantsToRemove.push_back(descendant);
        if ((int)descendant->m_children.size() > 0)
        {
            for (UIElement* child : descendant->m_children)
                RemoveChild(child);
        }
        else
            descendant = nullptr;
    }

    for (UIElement* descendantToRemove : m_descendantsToRemove)
        delete descendantToRemove;
    m_descendantsToRemove.clear();
}

//*******************************************************************
UIElement* UIElement::GetParent()
{
    return m_parent;
}

//*******************************************************************
void UIElement::DebugRender(Rgba color)
{
    AABB2D uiElementBounds = GetElementBounds();
    Matrix4 worldTransform = GetWorldTransform();
    Vector2 transformedMins = MultiplyVector2ByMatrix4(uiElementBounds.m_mins, worldTransform);
    Vector2 transformedMaxs = MultiplyVector2ByMatrix4(uiElementBounds.m_maxs, worldTransform);
    g_theSimpleRenderer->DrawQuad(transformedMins, transformedMaxs, Vector2(0.0f, 1.0f), Vector2(1.0f, 0.0f), color);
}

//*******************************************************************
void UIElement::Render()
{
}

//*******************************************************************
void UIElement::RenderChildElements()
{
    for (UIElement* child : m_children)
    {
        child->Render();
        if ((int)child->m_children.size() > 0)
            child->RenderChildElements();
    }
        
}

//*******************************************************************
void UIElement::RemoveSelf()
{
    for (UIElement* child : m_children)
        delete child;
    m_children.clear();

    delete this;
}
