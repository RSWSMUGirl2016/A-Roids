#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/MeshBuilder/OBJLoader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

//*******************************************************************
OBJLoader::OBJLoader()
    : m_path(nullptr)
{
}

//*******************************************************************
OBJLoader::OBJLoader(char const *objFilePath)
    : m_path(nullptr)
{
    bool wasFileFound = DidOBJFileLoad(objFilePath);
    if (wasFileFound == false)
    {
        return;
    }

    m_path = objFilePath;
    ReadOBJFile();
}

//*******************************************************************
OBJLoader::~OBJLoader()
{
}

//*******************************************************************
bool OBJLoader::DidOBJFileLoad(char const *objFilePath)
{
    std::ifstream inputFileStream;
    inputFileStream.open(objFilePath);
    if (!inputFileStream.is_open())
    {
        DebuggerPrintf("Cannot open OBJ file");
        return false;
    }
    inputFileStream.close();
    return true;
}

//*******************************************************************
void OBJLoader::ReadOBJFile()
{
    std::ifstream inputFileStream;
    inputFileStream.open(m_path);

    while (!inputFileStream.eof())
    {
        std::string line;
        std::getline(inputFileStream, line);

        if (!inputFileStream)
            return;
        if (line[0] == 'v' && line[1] == ' ')
            AddVertex(line);
        if (line[0] == 'v' && line[1] == 't')
            AddVertexTextureCoordinate(line);
        if (line[0] == 'v' && line[1] == 'n')
            AddVertexNormal(line);
        if (line[0] == 'f')
            AddTriangleVertex(line);
    }
}

//*******************************************************************
void OBJLoader::AddVertex(std::string vertexLine)
{
    std::istringstream iStringStream(vertexLine);
    std::string stringToken;

    int currentIndex = 0;
    Vector3 vertex;

    while (std::getline(iStringStream, stringToken, ' '))
    {
        if(stringToken == "v")
            continue;

        std::string::size_type size;
        float vertexCoordinate = std::stof(stringToken, &size);

        if (currentIndex == 0)
            vertex.x = vertexCoordinate;
        if (currentIndex == 1)
            vertex.y = vertexCoordinate;
        if (currentIndex == 2)
            vertex.z = vertexCoordinate;

        currentIndex++;
    }

    m_outVertices.push_back(vertex);
}

//*******************************************************************
void OBJLoader::AddVertexTextureCoordinate(std::string vertexLine)
{
    std::istringstream iStringStream(vertexLine);
    std::string stringToken;

    int currentIndex = 0;
    Vector2 vertexTextureCoordinate;

    while (std::getline(iStringStream, stringToken, ' '))
    {
        if (stringToken == "vt")
            continue;

        std::string::size_type size;
        float textureCoordinate = std::stof(stringToken, &size);

        if (currentIndex == 0)
            vertexTextureCoordinate.x = textureCoordinate;
        if (currentIndex == 1)
            vertexTextureCoordinate.y = 1.0f - textureCoordinate;

        currentIndex++;
    }

    m_outUVs.push_back(vertexTextureCoordinate);
}

//*******************************************************************
void OBJLoader::AddVertexNormal(std::string vertexLine)
{
    std::istringstream iStringStream(vertexLine);
    std::string stringToken;

    int currentIndex = 0;
    Vector3 vertexNormal;

    while (std::getline(iStringStream, stringToken, ' '))
    {
        if (stringToken == "vn")
            continue;

        std::string::size_type size;
        float faceNormal = std::stof(stringToken, &size);

        if (currentIndex == 0)
            vertexNormal.x = faceNormal;
        if (currentIndex == 1)
            vertexNormal.y = faceNormal;
        if (currentIndex == 2)
            vertexNormal.z = faceNormal;

        currentIndex++;
    }

    m_outNormals.push_back(vertexNormal);
}

//*******************************************************************
void OBJLoader::AddTriangleVertex(std::string vertexLine)
{
    std::istringstream iStringStream(vertexLine);
    std::string stringToken;

    int currentTriangleIndex = 0;
    Vertex3 triangleVertex;

    while (std::getline(iStringStream, stringToken, ' '))
    {
        if (stringToken == "f")
            continue;

        std::istringstream iStringStream2(stringToken);
        std::string stringToken2;

        int vertexIndex = 0;
        Vector3 position;
        Vector2 uv;
        Vector3 normal;

        while (std::getline(iStringStream2, stringToken2, '/'))
        {
            if (stringToken2 == "")
            {
                vertexIndex++;
                continue;
            }
                

            std::string::size_type size;
            int index = std::stoi(stringToken2, &size);

            if (vertexIndex == 0)
                position = m_outVertices[index - 1];
            if (vertexIndex == 1)
                uv = m_outUVs[index - 1];              
            if (vertexIndex == 2)
                normal = m_outNormals[index - 1];

            vertexIndex++;
        }

        triangleVertex = Vertex3(position, uv, normal);
        

        if (currentTriangleIndex == 3)
        {
            Vertex3 triangleVertex2 = m_outTriangleVertices[(int)m_outTriangleVertices.size() - 3];
            Vertex3 triangleVertex3 = m_outTriangleVertices[(int)m_outTriangleVertices.size() - 1];

            m_outTriangleVertices.push_back(triangleVertex2);
            m_outTriangleVertices.push_back(triangleVertex3);
            m_outTriangleVertices.push_back(triangleVertex);            
        }
        else
        {
            m_outTriangleVertices.push_back(triangleVertex);
        }

        currentTriangleIndex++;
    }
}