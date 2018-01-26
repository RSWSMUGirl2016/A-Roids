#pragma once
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Math/Matrix4.hpp"
#include "Engine/Core/Rgba.hpp"
#include <vector>

struct UIMetricT
{
    UIMetricT()
        : m_ratio(Vector2())
        , m_unit(Vector2())
    {}

    Vector2 m_ratio;
    Vector2 m_unit;
};

class UIElement
{
public:
    UIMetricT m_position;
    Vector2 m_pivot; //ratio of my bounds
    UIMetricT m_size;

    AABB2D m_bounds;
    
    UIElement* m_parent;
    std::vector<UIElement*> m_children;

public:
    UIElement();
    ~UIElement();

    void SetPosition(Vector2 ratio, Vector2 unit);
    void SetSize(Vector2 size);
    void SetSizeRatio(Vector2 sizeRatio);
    void SetPivot(Vector2 ratioInRespectToMyBounds);

    Vector2 GetSize();
    AABB2D GetElementBounds();
    AABB2D GetLocalBounds();
    Vector2 GetLocalPosition();
    Matrix4 GetLocalTransform();
    Matrix4 GetWorldTransform();

    UIElement* AddChild(UIElement* newChild);
    void RemoveChild(UIElement* childToRemove);
    UIElement* GetParent();

    template <typename T> T* CreateChild()
    {
        return (T*) AddChild(new T());
    }

    void DebugRender(Rgba color);

    virtual void Render();
    void RenderChildElements();

private:
    void RemoveSelf();
    std::vector<UIElement*> m_descendantsToRemove;
};