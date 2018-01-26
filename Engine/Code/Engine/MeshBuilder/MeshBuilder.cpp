#include "Engine/MeshBuilder/MeshBuilder.hpp"

//*******************************************************************
MeshBuilder::MeshBuilder()
{
    //vec3::zero,      // position
    //vec3::right,     // tangent
    //vec3::up,        // bitangent
    //-vec3::forward,  // normal, pointing toward camera camera
    //color_fl::white, // white vertex coloring (so no coloring basically)
    //vec2::zero,      // First UV
    //vec2::zero       // Second UV

    //position, normal, tangent, bitangent, rgba, uv1, uv2
    m_vertexStamp = Vertex3(Vector3(), Vector3(0.f, 0.f, 1.f), Vector3(1.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), Rgba::WHITE, Vector2(), Vector2(), UnsignedIntVector4(), Vector4());
}

//*******************************************************************
MeshBuilder::~MeshBuilder()
{
}

//*******************************************************************
void MeshBuilder::SetTanget(Vector3 const &tangent)
{
    m_vertexStamp.m_tangent = tangent;
}

//*******************************************************************
void MeshBuilder::SetBitanget(Vector3 const &bitangent)
{
    m_vertexStamp.m_bitangent = bitangent;
}

//*******************************************************************
void MeshBuilder::SetNormal(Vector3 const &normal)
{
    m_vertexStamp.m_normal = normal;
}

//*******************************************************************
void MeshBuilder::SetColor(Rgba const &color)
{
    m_vertexStamp.m_rgba = color;
}

//*******************************************************************
void MeshBuilder::SetUV(Vector2 const &uv)
{
    m_vertexStamp.m_uv = uv;
}

//*******************************************************************
void MeshBuilder::SetUV(size_t uvIdx, Vector2 const &uv)
{
    m_vertexStamp.m_uvs[uvIdx] = uv;
}

//*******************************************************************
void MeshBuilder::SetBoneIndices(UnsignedIntVector4 boneIndices)
{
    m_vertexStamp.m_boneIndices = boneIndices;
}

//*******************************************************************
void MeshBuilder::SetBoneWeights(Vector4 boneWeights)
{
    m_vertexStamp.m_boneWeights = boneWeights;
}

//*******************************************************************
size_t MeshBuilder::AddVertex(Vector3 const &position)
{
    m_vertexStamp.m_position = position;
    m_vertices.push_back(m_vertexStamp);
    return m_vertices.size() - 1;
}

//*******************************************************************
void MeshBuilder::Clear()
{
    m_vertices.clear();
    m_indices.clear();
    m_drawInstructions.clear();
}

//*******************************************************************
void MeshBuilder::FlipX()
{
    // You just want to flip the X coordinate of all vertex
    // data (positions,normals/tangents/bitangents)

    for (int vertexIndex = 0; vertexIndex < (int)m_vertices.size(); vertexIndex++)
    {
        m_vertices[vertexIndex].m_position.x = m_vertices[vertexIndex].m_position.x * -1.f;
        m_vertices[vertexIndex].m_normal.x = m_vertices[vertexIndex].m_normal.x * -1.f;
        m_vertices[vertexIndex].m_tangent.x = m_vertices[vertexIndex].m_tangent.x * -1.f;
        m_vertices[vertexIndex].m_bitangent.x = m_vertices[vertexIndex].m_bitangent.x * -1.f;
    }
}

//*******************************************************************
void MeshBuilder::Begin(ePrimitiveType type, bool usesAnIndexBuffer)
{
    //m_startIndex, number of verts in m_vertices
    //m_count, number of verts made in draw call

    drawInstruction drawInstr;
    drawInstr.m_startIndex = (unsigned int)m_vertices.size() - 1;
    drawInstr.m_type = type;
    drawInstr.m_usesIndexBuffer = usesAnIndexBuffer;
    m_drawInstructions.push_back(drawInstr);
}

//*******************************************************************
void MeshBuilder::Begin(char const* materialName, ePrimitiveType type, bool usesAnIndexBuffer)
{
    drawInstruction drawInstr;
    drawInstr.m_startIndex = (unsigned int)m_vertices.size() - 1;
    drawInstr.m_type = type;
    drawInstr.m_usesIndexBuffer = usesAnIndexBuffer;
    m_drawInstructions.push_back(drawInstr);

    m_materialName = materialName;
}

//*******************************************************************
void MeshBuilder::End(unsigned int vertexCount)
{
    m_drawInstructions[m_drawInstructions.size() - 1].m_count = vertexCount;
}
