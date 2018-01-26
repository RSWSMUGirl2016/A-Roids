#pragma once
#include "Engine/RHI/IndexBuffer.hpp"
#include "Engine/RHI/VertexBuffer.hpp"

#include <vector>

struct drawInstruction
{
    unsigned int    m_startIndex;
    unsigned int    m_count;
    ePrimitiveType  m_type;
    bool            m_usesIndexBuffer;
};

class Mesh
{
    //Optional CPU Buffers
    std::vector<Vertex3> m_vertices;
    std::vector<unsigned int> m_indices;

    std::vector<drawInstruction> m_drawInstructions;

    //GPU Buffers
    VertexBuffer *m_vbo;
    IndexBuffer *m_ibo;

    Mesh();
    ~Mesh();
};