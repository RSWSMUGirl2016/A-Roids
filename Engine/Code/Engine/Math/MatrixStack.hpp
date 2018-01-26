#pragma once
#include "Engine/Math/Matrix4.hpp"
#include <vector>

class MatrixStack
{
public:
    Matrix4 m_top;
    std::vector<Matrix4> m_stack;

    MatrixStack();
    ~MatrixStack();

    void PushMultiply(Matrix4 matrix);
    void Push(Matrix4 matrix);
    void Pop();
};