#pragma once
#include <vector>
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vertex3.hpp"
#include <string>

class OBJLoader
{
public:
    OBJLoader();
    OBJLoader(char const *objFilePath);
    ~OBJLoader();

    bool DidOBJFileLoad(char const *objFilePath);
    void ReadOBJFile();    

    void AddVertex(std::string vertexLine);
    void AddVertexTextureCoordinate(std::string vertexLine);
    void AddVertexNormal(std::string vertexLine);
    void AddTriangleVertex(std::string vertexLine);

    const char *m_path;
    std::vector<Vector3> m_outVertices;
    std::vector<Vector2> m_outUVs;
    std::vector<Vector3> m_outNormals;
    std::vector<Vertex3> m_outTriangleVertices;
};
