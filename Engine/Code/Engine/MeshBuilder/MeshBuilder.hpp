#pragma once
#include "Engine/RHI/RHITypes.hpp"

#include "Engine/MeshBuilder/Mesh.hpp"

#include <vector>

class MeshBuilder
{
public:
    std::vector<Vertex3> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<drawInstruction> m_drawInstructions;

    char const *m_materialName;
    
private:
    Vertex3 m_vertexStamp;

public:
    MeshBuilder();
    ~MeshBuilder();

    void SetTanget(Vector3 const &tangent);
    void SetBitanget(Vector3 const &bitangent);
    void SetNormal(Vector3 const &normal);
    void SetColor(Rgba const &color);
    void SetUV(Vector2 const &uv);
    void SetUV(size_t uvIdx, Vector2 const &uv);
    void SetBoneIndices(UnsignedIntVector4 boneIndices);
    void SetBoneWeights(Vector4 boneWeights);

    size_t AddVertex(Vector3 const &position);

    void Clear();
    void FlipX();
    void Begin(ePrimitiveType type, bool usesAnIndexBuffer);
    void Begin(char const* materialName, ePrimitiveType type, bool usesAnIndexBuffer);
    void End(unsigned int vertexCount);
};