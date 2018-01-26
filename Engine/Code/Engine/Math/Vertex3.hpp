#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/UnsignedIntVector4.hpp"
#include "Engine/Core/Rgba.hpp"

#define VERTEX_MAX_UVS (4)
#define MAX_BUILDER_UVS (2)

float constexpr PI = (3.1415926535897932384626f);
#define D2R(deg) ((deg) * (PI / 180.0f))


class Vertex3
{
public:
	Vector3 m_position;
    Vector3 m_normal;
    Vector3 m_tangent;
    Vector3 m_bitangent;
    Rgba m_rgba;
    Vector2 m_uv;

    Vector2 m_uvs[MAX_BUILDER_UVS];
    UnsignedIntVector4 m_boneIndices;
    Vector4 m_boneWeights;

public:
	Vertex3() {};
	Vertex3(Vector3 const &position, Rgba const &rgba, Vector2 const &uv)
        : m_position(position)
        , m_normal(Vector3())
        , m_tangent(Vector3())
        , m_bitangent(Vector3())
        , m_rgba(rgba)
        , m_uv(uv)
	{};
    Vertex3(Vector3 const &position, Vector2 const &uv, Vector3 const &normal)
        : m_position(position)
        , m_normal(normal)
        , m_tangent(Vector3())
        , m_bitangent(Vector3())
        , m_rgba(Rgba())
        , m_uv(uv)
    {};
    Vertex3(Vector3 const &position, Vector3 const &normal, Vector3 const &tangent, Vector3 const &bitangent, Rgba const &rgba, Vector2 const &uv)
        : m_position(position)
        , m_normal(normal)
        , m_tangent(tangent)
        , m_bitangent(bitangent)
        , m_rgba(rgba)
        , m_uv(uv)
    {};
    Vertex3(Vector3 const &position, Vector3 const &normal, Vector3 const &tangent, Vector3 const &bitangent, Rgba const &rgba, Vector2 const &uv1, Vector2 const &uv2)
        : m_position(position)
        , m_normal(normal)
        , m_tangent(tangent)
        , m_bitangent(bitangent)
        , m_rgba(rgba)
        , m_uv(uv1)
    {
        m_uvs[0] = uv1;
        m_uvs[1] = uv2;
    };
    Vertex3(Vector3 const &position, Vector2 const &uv, Vector3 const &normal, UnsignedIntVector4 boneIndices, Vector4 boneWeights)
        : m_position(position)
        , m_normal(normal)
        , m_tangent(Vector3())
        , m_bitangent(Vector3())
        , m_rgba(Rgba())
        , m_uv(uv)
        , m_boneIndices(boneIndices)
        , m_boneWeights(boneWeights)
    {};
    Vertex3(Vector3 const &position, Vector3 const &normal, Vector3 const &tangent, Vector3 const &bitangent, Rgba const &rgba, Vector2 const &uv1, Vector2 const &uv2, UnsignedIntVector4 boneIndices, Vector4 boneWeights)
        : m_position(position)
        , m_normal(normal)
        , m_tangent(tangent)
        , m_bitangent(bitangent)
        , m_rgba(rgba)
        , m_uv(uv1)
        , m_boneIndices(boneIndices)
        , m_boneWeights(boneWeights)
    {
        m_uvs[0] = uv1;
        m_uvs[1] = uv2;
    };

};