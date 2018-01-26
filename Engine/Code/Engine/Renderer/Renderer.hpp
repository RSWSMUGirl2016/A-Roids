#pragma once
#pragma comment( lib, "opengl32" ) // Link in the OpenGL32.lib static library
#pragma comment( lib, "Glu32" ) // Link in the Glu.lib static library
#define WIN32_LEAN_AND_MEAN

#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Math/Disc2D.hpp"
#include <string>
#include <vector>
#include <map>
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/BitmapFont.hpp"

extern unsigned int PRIMITIVE_POINTS;
extern unsigned int PRIMITIVE_LINES;
extern unsigned int PRIMITIVE_LINE_LOOP;
extern unsigned int PRIMITIVE_QUADS;
extern unsigned int PRIMITIVE_POLYGONS;
extern unsigned int PRIMITIVE_TRIANGLES;

struct Vertex2D_PC
{
	Vector2		pos;
	Rgba		color;
};

struct Vertex3D_PC
{
	Vector3		pos;
	Rgba		color;
};

struct Vertex2_PCT
{
	Vertex2_PCT() {};
	Vertex2_PCT(const Vector2& position, const Rgba& color, const Vector2& texCoords)
		: m_position(position)
		, m_color(color)
		, m_texCoords(texCoords)
	{};

	Vector2 m_position;
	Rgba m_color;
	Vector2 m_texCoords;
};

struct Vertex3_PCT
{
	Vertex3_PCT() {};
	Vertex3_PCT(const Vector3& position, const Rgba& color, const Vector2& texCoords)
		: m_position(position)
		, m_color(color)
		, m_texCoords(texCoords)
	{};

	Vector3 m_position;
	Rgba m_color;
	Vector2 m_texCoords;
};

class Renderer
{
private:
	std::vector< Texture* >		m_alreadyLoadedTextures;
	std::map<std::string, BitmapFont*> m_loadedFonts;
public:
	Renderer();
	~Renderer();
public:
	void ClearScreen(const Rgba& clearColor) const;
	void SetOrtho(const Vector2& bottomLeft, const Vector2& topRight) const;
	void SetPerspective(float fovYDegrees, float aspectRatio, float nearClipDist, float farClipDist);
	void DrawVertex2D_PC(Vertex2D_PC* vertexArray, int numVertexes, unsigned int primitiveDrawMode) const;
	void DrawVertex3D_PC(Vertex3D_PC* vertexArray, int numVertexes, unsigned int primitiveDrawMode) const;
	void DrawVertex3D_PC(const Vertex3D_PC* vertexArray, int numVertexes, unsigned int primitiveDrawMode) const;
	void DrawVertex2_PCT(Vertex2_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture) const;
	void DrawVertex3_PCT(Vertex3_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture) const;
	void DrawVertexArray(const Vertex3_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture);
	void DrawVertexArray(const Vertex3_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture) const;

public:
	unsigned int CreateVBO();
	void UpdateVBO(unsigned int vboID, Vertex3_PCT* vertexArray, int numVertexes);
	void DrawVBO3D_PCT(unsigned int vboID, int numVertexes, unsigned int primitiveDrawMode);
	void DestroyVBO(unsigned int vboID);	
	
public:
	void DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& startColor, const Rgba& endColor, float lineThickness) const;
	void DrawLine3D(const Vector3& start, const Vector3& end, const Rgba& startColor, const Rgba& endColor, float lineThickness) const;
	void DrawXRayLine3D(const Vector3& start, const Vector3& end, Rgba& color, float lineThickness) const;
	void DrawOriginLine(float axisLength, char axis, Rgba color) const;

public:
	void DrawAABB2D(const AABB2D& boxCooridnates, float orientationDegrees, const Rgba& color) const;
	void DrawAABB2D(const AABB2D& boxCooridnates, float orientationDegrees, float scale, const Rgba& color) const;
	void DrawNonFilledAABB2D(const AABB2D& boxCooridnates, float orientationDegrees, const Rgba& color) const;
	void DrawFilledCircle(const Disc2D& discMembers, const Rgba& color, float orientationDegrees) const;
	void DrawCircle(const Disc2D& discMembers, const Rgba& color) const;
	void DrawCircle(const Disc2D& discMembers, float scale, const Rgba& color) const;

public:
	Texture* CreateOrGetTexture(const std::string& imageFilePath);
	Texture* GetTexture(int textureId);
	void DrawTexturedAABB2D(const AABB2D& bounds, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint);
	void DrawTexturedAABB2D(const AABB2D& bounds, float orientationDegrees, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint);
	void DrawTexturedAABB2D(const AABB2D& bounds, const Texture& texture);
	void DrawTexturedAABB2D(const AABB2D& bounds, float orientationDegrees, float scale, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint);
	void SetAdditivieBlending();
	void SetAlphaBlending();
	void DrawAdditiveBlendedTexture(const AABB2D& bounds, float orientationDegrees, float scale, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint);

private:
	Texture* CreateTextureFromFile(const std::string& imageFilePath);
	Texture* GetTexture(const std::string& imageFilePath);
	unsigned int CreateOpenGLTexture(unsigned char* imageTexelBytes, int width, int height, int bytesPerTexel);

public:
	SpriteSheet CreateOrGetSpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh);
	BitmapFont* GetFont(const std::string& bitmapFontName);
	BitmapFont* CreateOrGetFont(const std::string& bitmapFontName);
	void DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint = Rgba::WHITE, const BitmapFont* font = nullptr);

public:
	void Translate3D(const Vector3& translation3D);
	void Rotate3D(float rotationDegrees, const Vector3& rotationAxis);
	void DrawTexturedQuad3D(const Vertex3_PCT& one, const Vertex3_PCT& two, const Vertex3_PCT& three, const Vertex3_PCT& four, const Texture& texture) const;
	void DrawVoxel(const Vector3& voxelCoordinates, const Texture& texture, const AABB2D& bottomVoxelFace, const AABB2D& sideVoxelFace, const AABB2D& topVoxelFace) const;

	void DrawVoxel(const Vector3& voxelCoordinates, const Texture& texture, const AABB2D& bottomVoxelFace, const AABB2D& sideVoxelFace, const AABB2D& topVoxelFace, const Rgba& tint) const;
	void DrawVoxel(const Vector3& voxelCoordinates, const Texture& texture, const AABB2D& bottomVoxelFace, const AABB2D& northSideVoxelFace, const AABB2D& southSideVoxelFace, const AABB2D& eastSideVoxelFace, const AABB2D& westSideVoxelFace, const AABB2D& topVoxelFace, const Rgba& tint) const;
public:
	void PushMatrix() const;
	void PopMatrix() const;
	void Translate(float x, float y, float z) const;
	void Rotate(float orientationDegrees, float x, float y, float z) const;
	void Scale(float x, float y, float z) const;
	void LoadIdentity() const;
	void Ortho(float left, float right, float bottom, float top, float zNear, float zFar) const;
	void ClearColor(float red, float green, float blue, float alpha) const;
	void EnableBlending() const;
	void EnableSmooth() const;
	void EnableLighting() const;
	void DisableLighting() const;
	void EnableColorMaterial() const;
	void DisableColorMaterial() const;
	void ColorMaterial() const;
	void AlphaBlendFunc() const;
	void LineThickness(float thickness) const;
	void PointSize(float size) const;
	void EnableDepthTesting();
	void DisableDepthTesting();
	void BindTexture(Texture* textureToBind);
	void Clear() const;
};
