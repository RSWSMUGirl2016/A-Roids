#include "Engine/Math/MatrixStack.hpp"

//*******************************************************************
MatrixStack::MatrixStack()
    : m_top(Matrix4())
{
}

//*******************************************************************
MatrixStack::~MatrixStack()
{
}

//*******************************************************************
//Equivalent to Forseth's push
void MatrixStack::PushMultiply(Matrix4 matrix)
{
    m_stack.push_back(m_top);
    matrix.ConcatenateTransform(m_top);
}

//*******************************************************************
//Equivalent to Forseth's push_direct
void MatrixStack::Push(Matrix4 matrix)
{
    m_stack.push_back(matrix);
    m_top = matrix;
}

//*******************************************************************
void MatrixStack::Pop()
{
    m_top = m_stack.front();
    m_stack.pop_back();
}
