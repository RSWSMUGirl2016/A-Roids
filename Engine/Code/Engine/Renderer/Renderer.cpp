#include "Renderer.hpp"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ThirdParty/OpenGL/glext.h"
#include "ThirdParty/OpenGL/wglext.h"
#include "Engine/Math/MathUtilities.hpp"
#include "ThirdParty/stb/stb_image.h"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Renderer/OpenGLExtensions.hpp"

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);

PFNGLGENBUFFERSPROC		glGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC	glDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC		glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC		glBufferData = nullptr;

unsigned int PRIMITIVE_POINTS = 0x0000;
unsigned int PRIMITIVE_LINES = 0x0001;
unsigned int PRIMITIVE_LINE_LOOP = 0x0002;
unsigned int PRIMITIVE_QUADS = 0X0007;
unsigned int PRIMITIVE_POLYGONS = 0x0009;
unsigned int PRIMITIVE_TRIANGLES = 0x0004;

//*******************************************************************
Renderer::Renderer()
{
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");	

	GUARANTEE_OR_DIE(glGenBuffers		!= nullptr, "ERROR: This video card does not support glGenBuffers!");
	GUARANTEE_OR_DIE(glDeleteBuffers	!= nullptr, "ERROR: This video card does not support glDeleteBuffers!");
	GUARANTEE_OR_DIE(glBindBuffer		!= nullptr, "ERROR: This video card does not support glBindBuffer!");
	GUARANTEE_OR_DIE(glBufferData		!= nullptr, "ERROR: This video card does not support glBufferData!");
}

//*******************************************************************
Renderer::~Renderer()
{
}

//*******************************************************************
void Renderer::ClearScreen(const Rgba& clearColor) const
{
	float r, g, b, a;
	clearColor.GetAsFloats(r, g, b, a);
	glClearColor(r, g, b, a);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_CULL_FACE);
}

//*******************************************************************
void Renderer::SetOrtho(const Vector2& bottomLeft, const Vector2& topRight) const
{
	LoadIdentity();
	Ortho(bottomLeft.x, topRight.x, bottomLeft.y, topRight.y, 0.f, 1.f);
}


//*******************************************************************
void Renderer::SetPerspective(float fovYDegrees, float aspectRatio, float nearClipDist, float farClipDist)
{
	glLoadIdentity();
	gluPerspective(fovYDegrees, aspectRatio, nearClipDist, farClipDist);
}

//*******************************************************************
void Renderer::DrawVertex2D_PC(Vertex2D_PC* vertexArray, int numVertexes, unsigned int primitiveDrawMode) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex2D_PC), &vertexArray[0].pos);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2D_PC), &vertexArray[0].color); // float RGBAs

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

//*******************************************************************
void Renderer::DrawVertex3D_PC(Vertex3D_PC* vertexArray, int numVertexes, unsigned int primitiveDrawMode) const
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D_PC), &vertexArray[0].pos);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3D_PC), &vertexArray[0].color); // float RGBAs

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

//*******************************************************************
void Renderer::DrawVertex3D_PC(const Vertex3D_PC* vertexArray, int numVertexes, unsigned int primitiveDrawMode) const
{
	glDisable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3D_PC), &vertexArray[0].pos);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3D_PC), &vertexArray[0].color); // float RGBAs

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

//*******************************************************************
void Renderer::DrawVertex2_PCT(Vertex2_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex2_PCT), &vertexArray[0].m_position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex2_PCT), &vertexArray[0].m_color); // float RGBAs
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex2_PCT), &vertexArray[0].m_texCoords);

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//*******************************************************************
void Renderer::DrawVertex3_PCT(Vertex3_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture) const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray[0].m_position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3_PCT), &vertexArray[0].m_color); // float RGBAs
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray[0].m_texCoords);

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//*******************************************************************
void Renderer::DrawVertexArray(const Vertex3_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture)
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray[0].m_position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3_PCT), &vertexArray[0].m_color); // float RGBAs
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray[0].m_texCoords);

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//*******************************************************************
void Renderer::DrawVertexArray(const Vertex3_PCT* vertexArray, int numVertexes, unsigned int primitiveDrawMode, const Texture& texture) const
{
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindTexture(GL_TEXTURE_2D, texture.m_textureID);
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray[0].m_position);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3_PCT), &vertexArray[0].m_color); // float RGBAs
	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), &vertexArray[0].m_texCoords);

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//*******************************************************************
void Renderer::DrawVBO3D_PCT(unsigned int vboID, int numVertexes, unsigned int primitiveDrawMode)
{
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex3_PCT), (const GLvoid*) offsetof(Vertex3_PCT, m_position));
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vertex3_PCT), (const GLvoid*) offsetof(Vertex3_PCT, m_color));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex3_PCT), (const GLvoid*) offsetof(Vertex3_PCT, m_texCoords));

	glDrawArrays(primitiveDrawMode, 0, numVertexes);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//*******************************************************************
void Renderer::UpdateVBO(unsigned int vboID, Vertex3_PCT* vertexArray, int numVertexes)
{
	size_t vertexArrayNumBytes = numVertexes * sizeof(Vertex3_PCT);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, vertexArrayNumBytes, vertexArray, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//*******************************************************************
unsigned int Renderer::CreateVBO()
{
	unsigned int vboID;
	glGenBuffers(1, &vboID);
	return vboID;
}

//*******************************************************************
void Renderer::DestroyVBO(unsigned int vboID)
{
	glDeleteBuffers(1, &vboID);
}

//*******************************************************************
void Renderer::DrawLine2D(const Vector2& start, const Vector2& end, const Rgba& startColor, const Rgba& endColor, float lineThickness) const
{
	UNUSED(lineThickness);
	const int NUM_VERTS = 2;
	Vertex2D_PC vertexes[NUM_VERTS];
	vertexes[0].pos = start;
	vertexes[0].color = startColor;
	vertexes[1].pos = end;
	vertexes[1].color = endColor;

	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], 2, PRIMITIVE_LINES);
}

//*******************************************************************
void Renderer::DrawLine3D(const Vector3& start, const Vector3& end, const Rgba& startColor, const Rgba& endColor, float lineThickness) const
{
	UNUSED(lineThickness);
	const int NUM_VERTS = 2;
	Vertex3D_PC vertexes[NUM_VERTS];
	vertexes[0].pos = start;
	vertexes[0].color = startColor;
	vertexes[1].pos = end;
	vertexes[1].color = endColor;

	glDisable(GL_TEXTURE_2D);
	DrawVertex3D_PC(&vertexes[0], 2, PRIMITIVE_LINES);
}

//*******************************************************************
void Renderer::DrawXRayLine3D(const Vector3& start, const Vector3& end, Rgba& color, float lineThickness) const
{
	glDisable(GL_DEPTH_TEST);
	color.a = 77;

	DrawLine3D(start, end, color, color, lineThickness);

	glEnable(GL_DEPTH_TEST);
	LineThickness(3.f);
	color.a = 255;

	DrawLine3D(start, end, color, color, lineThickness);

	LineThickness(1.f);
}

//*******************************************************************
void Renderer::DrawOriginLine(float axisLength, char axis, Rgba color) const
{
	glDisable(GL_DEPTH_TEST);

	color.a = 77;
	if (axis == 'X')
	{
		DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(axisLength, 0.f, 0.f), color, color, 0.3f);
	}
	if (axis == 'Y')
	{
		DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, axisLength, 0.f), color, color, 0.3f);
	}
	if (axis == 'Z')
	{
		DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, axisLength), color, color, 0.3f);
	}		

	glEnable(GL_DEPTH_TEST);
	LineThickness(3.f);

	color.a = 255;
	if (axis == 'X')
	{
		DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(axisLength, 0.f, 0.f), color, color, 1.f);
	}
	if (axis == 'Y')
	{
		DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, axisLength, 0.f), color, color, 1.f);
	}		
	if (axis == 'Z')
	{
		DrawLine3D(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, axisLength), color, color, 1.f);
	}		

	LineThickness(1.f);
}

//*******************************************************************
void Renderer::DrawFilledCircle(const Disc2D& discMembers, const Rgba& color, float orientationDegrees) const
{
	std::vector<Vertex2D_PC> vertexes;
	glPushMatrix();
	glTranslatef(discMembers.center.x, discMembers.center.y, 0.f);
	glRotatef(orientationDegrees, 0.f, 0.f, 1.f);
	glScalef(discMembers.radius, discMembers.radius, 1.f);
	const float DEGREES_PER_VERTEX = 360.f / 100.f;
	for (int vertexIndex = 0; vertexIndex < 100; ++vertexIndex)
	{
        float degrees = DEGREES_PER_VERTEX * (float)vertexIndex;
        float x = CosDegrees(degrees);
        float y = SinDegrees(degrees);

		Vertex2D_PC currentVertex;
		currentVertex.pos = Vector2(x, y);
		currentVertex.color = color;
		vertexes.push_back(currentVertex);
	}
	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], vertexes.size(), PRIMITIVE_POLYGONS);
	glPopMatrix();
}


//*******************************************************************
void Renderer::DrawCircle(const Disc2D& discMembers, const Rgba& color) const
{
	std::vector<Vertex2D_PC> vertexes;

	glPushMatrix();
	glTranslatef(discMembers.center.x, discMembers.center.y, 0.f);
	glScalef(discMembers.radius, discMembers.radius, 1.f);
	const float DEGREES_PER_VERTEX = 360.f / 100.f;
	for (int vertexIndex = 0; vertexIndex < 100; ++vertexIndex)
	{
		float degrees = DEGREES_PER_VERTEX * (float)vertexIndex;
		float x = CosDegrees(degrees);
		float y = SinDegrees(degrees);
		
		Vertex2D_PC currentVertex;
		currentVertex.pos = Vector2(x, y);
		currentVertex.color = color;
		vertexes.push_back(currentVertex);
	}
	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], vertexes.size(), PRIMITIVE_LINE_LOOP);
	glPopMatrix();
}

//*******************************************************************
void Renderer::DrawCircle(const Disc2D& discMembers, float scale, const Rgba& color) const
{
	std::vector<Vertex2D_PC> vertexes;
	glPushMatrix();
	glTranslatef(discMembers.center.x, discMembers.center.y, 0.f);
	glScalef(scale * discMembers.radius, scale * discMembers.radius, 1.f);
	const float DEGREES_PER_VERTEX = 360.f / 100.f;
	for (int vertexIndex = 0; vertexIndex < 100; ++vertexIndex)
	{
		float degrees = DEGREES_PER_VERTEX * (float)vertexIndex;
		float x = CosDegrees(degrees);
		float y = SinDegrees(degrees);

		Vertex2D_PC currentVertex;
		currentVertex.pos = Vector2(x, y);
		currentVertex.color = color;
		vertexes.push_back(currentVertex);
	}
	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], vertexes.size(), PRIMITIVE_POLYGONS);
	glPopMatrix();
}


//*******************************************************************
void Renderer::DrawAABB2D(const AABB2D& boxCooridnates, float orientationDegrees, const Rgba& color) const
{
	UNUSED(orientationDegrees);

	const int NUM_VERTS = 4;
	Vertex2D_PC vertexes[NUM_VERTS];
	vertexes[0].pos = boxCooridnates.m_mins;
	vertexes[0].color = color;
	vertexes[1].pos = Vector2(boxCooridnates.m_maxs.x, boxCooridnates.m_mins.y);
	vertexes[1].color = color;
	vertexes[2].pos = boxCooridnates.m_maxs;
	vertexes[2].color = color;
	vertexes[3].pos = Vector2(boxCooridnates.m_mins.x, boxCooridnates.m_maxs.y);
	vertexes[3].color = color;

	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS);
}

//*******************************************************************
void Renderer::DrawAABB2D(const AABB2D& boxCooridnates, float orientationDegrees, float scale, const Rgba& color) const
{
	UNUSED(scale);

	glPushMatrix();
	glRotatef(orientationDegrees, 0.f, 0.f, 1.f);
	//glTranslatef(boxCooridnates.CalcCenter().x, boxCooridnates.CalcCenter().y, 0.f);
	//glScalef(boxCooridnates.CalcSize().x, boxCooridnates.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2D_PC vertexes[NUM_VERTS];
	vertexes[0].pos = boxCooridnates.m_mins;
	vertexes[0].color = color;
	vertexes[1].pos = Vector2(boxCooridnates.m_maxs.x, boxCooridnates.m_mins.y);
	vertexes[1].color = color;
	vertexes[2].pos = boxCooridnates.m_maxs;
	vertexes[2].color = color;
	vertexes[3].pos = Vector2(boxCooridnates.m_mins.x, boxCooridnates.m_maxs.y);
	vertexes[3].color = color;

	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS);
	glPopMatrix();
}

//*******************************************************************
void Renderer::DrawNonFilledAABB2D(const AABB2D& boxCooridnates, float orientationDegrees, const Rgba& color) const
{
	UNUSED(orientationDegrees);

	glPushMatrix();	
	//glTranslatef(boxCooridnates.CalcCenter().x, boxCooridnates.CalcCenter().y, 0.f);
	glRotatef(orientationDegrees, 0.f, 0.f, 1.f);
	//glScalef(boxCooridnates.CalcSize().x, boxCooridnates.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2D_PC vertexes[NUM_VERTS];
	vertexes[0].pos = boxCooridnates.m_mins;
	vertexes[0].color = color;
	vertexes[1].pos = Vector2(boxCooridnates.m_maxs.x, boxCooridnates.m_mins.y);
	vertexes[1].color = color;
	vertexes[2].pos = boxCooridnates.m_maxs;
	vertexes[2].color = color;
	vertexes[3].pos = Vector2(boxCooridnates.m_mins.x, boxCooridnates.m_maxs.y);
	vertexes[3].color = color;

	glDisable(GL_TEXTURE_2D);
	DrawVertex2D_PC(&vertexes[0], NUM_VERTS, PRIMITIVE_LINES);
	glPopMatrix();
}

//*******************************************************************
void Renderer::DrawTexturedAABB2D(const AABB2D& bounds, const Texture& texture)
{
	glPushMatrix();
	//glTranslatef(bounds.CalcCenter().x, bounds.CalcCenter().y, 0.f);
	//glScalef(bounds.CalcSize().x, bounds.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2_PCT vertexes[NUM_VERTS];
	Rgba tint(255, 255, 255, 255);
	Vector2 textureCoordinatesAtMins(0.f, 1.f);
	Vector2 textureCoordinatesAtMaxs(1.f, 0.f);

	vertexes[0].m_position = bounds.m_mins;
	vertexes[0].m_color = tint;
	vertexes[0].m_texCoords = textureCoordinatesAtMins;

	vertexes[1].m_position = Vector2(bounds.m_maxs.x, bounds.m_mins.y);
	vertexes[1].m_color = tint;
	vertexes[1].m_texCoords = Vector2(textureCoordinatesAtMaxs.x, textureCoordinatesAtMins.y);

	vertexes[2].m_position = bounds.m_maxs;
	vertexes[2].m_color = tint;
	vertexes[2].m_texCoords = textureCoordinatesAtMaxs;

	vertexes[3].m_position = Vector2(bounds.m_mins.x, bounds.m_maxs.y);
	vertexes[3].m_color = tint;
	vertexes[3].m_texCoords = Vector2(textureCoordinatesAtMins.x, textureCoordinatesAtMaxs.y);

	glEnable(GL_TEXTURE_2D);
	DrawVertex2_PCT(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS, texture);
	glPopMatrix();
}



//*******************************************************************
// Credit to Squirrel Eiserloh
Texture* Renderer::CreateOrGetTexture(const std::string& imageFilePath)
{
	// Try to find that texture from those already loaded
	Texture* texture = GetTexture(imageFilePath);
	if (texture)
		return texture;

	texture = CreateTextureFromFile(imageFilePath);
	return texture;
}

//*******************************************************************
Texture* Renderer::GetTexture(int textureId)
{
	if (m_alreadyLoadedTextures.empty())
		return nullptr;
	if ((int)m_alreadyLoadedTextures.size() >= textureId)
		return nullptr;

	Texture* texture = m_alreadyLoadedTextures[textureId];
	return texture;
}


//*******************************************************************
// Credit to Squirrel Eiserloh
void Renderer::DrawTexturedAABB2D(const AABB2D& bounds, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint)
{
	glPushMatrix();
	//glTranslatef(bounds.CalcCenter().x, bounds.CalcCenter().y, 0.f);
	//glScalef(bounds.CalcSize().x, bounds.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2_PCT vertexes[NUM_VERTS];

	vertexes[0].m_position = bounds.m_mins;
	vertexes[0].m_color = tint;
	vertexes[0].m_texCoords = textureCoordinatesAtMins;

	vertexes[1].m_position = Vector2(bounds.m_maxs.x, bounds.m_mins.y);
	vertexes[1].m_color = tint;
	vertexes[1].m_texCoords = Vector2(textureCoordinatesAtMaxs.x, textureCoordinatesAtMins.y);

	vertexes[2].m_position = bounds.m_maxs;
	vertexes[2].m_color = tint;
	vertexes[2].m_texCoords = textureCoordinatesAtMaxs;

	vertexes[3].m_position = Vector2(bounds.m_mins.x, bounds.m_maxs.y);
	vertexes[3].m_color = tint;
	vertexes[3].m_texCoords = Vector2(textureCoordinatesAtMins.x, textureCoordinatesAtMaxs.y);

	glEnable(GL_TEXTURE_2D);
	DrawVertex2_PCT(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS, texture);
	glPopMatrix();
}

//*******************************************************************
void Renderer::DrawTexturedAABB2D(const AABB2D& bounds, float orientationDegrees, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint)
{
	//UNUSED(textureCoordinatesAtMins);
	//UNUSED(textureCoordinatesAtMaxs);
	glPushMatrix();
	//glTranslatef(bounds.CalcCenter().x, bounds.CalcCenter().y, 0.f);
	glRotatef(orientationDegrees, 0.f, 0.f, 1.f);
	//glScalef(bounds.CalcSize().x, bounds.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2_PCT vertexes[NUM_VERTS];

	vertexes[0].m_position = bounds.m_mins;
	vertexes[0].m_color = tint;
	vertexes[0].m_texCoords = textureCoordinatesAtMins;

	vertexes[1].m_position = Vector2(bounds.m_maxs.x, bounds.m_mins.y);
	vertexes[1].m_color = tint;
	vertexes[1].m_texCoords = Vector2(textureCoordinatesAtMaxs.x, textureCoordinatesAtMins.y);

	vertexes[2].m_position = bounds.m_maxs;
	vertexes[2].m_color = tint;
	vertexes[2].m_texCoords = textureCoordinatesAtMaxs;

	vertexes[3].m_position = Vector2(bounds.m_mins.x, bounds.m_maxs.y);
	vertexes[3].m_color = tint;
	vertexes[3].m_texCoords = Vector2(textureCoordinatesAtMins.x, textureCoordinatesAtMaxs.y);

	glEnable(GL_TEXTURE_2D);
	DrawVertex2_PCT(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS, texture);
	glPopMatrix();
}

//*******************************************************************
void Renderer::DrawTexturedAABB2D(const AABB2D& bounds, float orientationDegrees, float scale, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint)
{
	UNUSED(scale);

	glPushMatrix();
	//glTranslatef(bounds.CalcCenter().x, bounds.CalcCenter().y, 0.f);
	glRotatef(orientationDegrees, 0.f, 0.f, 1.f);
	//glScalef(bounds.CalcSize().x, bounds.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2_PCT vertexes[NUM_VERTS];

	vertexes[0].m_position = bounds.m_mins;
	vertexes[0].m_color = tint;
	vertexes[0].m_texCoords = textureCoordinatesAtMins;

	vertexes[1].m_position = Vector2(bounds.m_maxs.x, bounds.m_mins.y);
	vertexes[1].m_color = tint;
	vertexes[1].m_texCoords = Vector2(textureCoordinatesAtMaxs.x, textureCoordinatesAtMins.y);

	vertexes[2].m_position = bounds.m_maxs;
	vertexes[2].m_color = tint;
	vertexes[2].m_texCoords = textureCoordinatesAtMaxs;

	vertexes[3].m_position = Vector2(bounds.m_mins.x, bounds.m_maxs.y);
	vertexes[3].m_color = tint;
	vertexes[3].m_texCoords = Vector2(textureCoordinatesAtMins.x, textureCoordinatesAtMaxs.y);

	glEnable(GL_TEXTURE_2D);
	DrawVertex2_PCT(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS, texture);
	glPopMatrix();
}

//*******************************************************************
void Renderer::SetAdditivieBlending()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

//*******************************************************************
void Renderer::SetAlphaBlending()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//*******************************************************************
void Renderer::DrawAdditiveBlendedTexture(const AABB2D& bounds, float orientationDegrees, float scale, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint)
{
	UNUSED(scale);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glPushMatrix();
	//glTranslatef(bounds.CalcCenter().x, bounds.CalcCenter().y, 0.f);
	glRotatef(orientationDegrees, 0.f, 0.f, 1.f);
	//glScalef(bounds.CalcSize().x, bounds.CalcSize().y, 1.f);
	const int NUM_VERTS = 4;
	Vertex2_PCT vertexes[NUM_VERTS];

	vertexes[0].m_position = bounds.m_mins;
	vertexes[0].m_color = tint;
	vertexes[0].m_texCoords = textureCoordinatesAtMins;

	vertexes[1].m_position = Vector2(bounds.m_maxs.x, bounds.m_mins.y);
	vertexes[1].m_color = tint;
	vertexes[1].m_texCoords = Vector2(textureCoordinatesAtMaxs.x, textureCoordinatesAtMins.y);

	vertexes[2].m_position = bounds.m_maxs;
	vertexes[2].m_color = tint;
	vertexes[2].m_texCoords = textureCoordinatesAtMaxs;

	vertexes[3].m_position = Vector2(bounds.m_mins.x, bounds.m_maxs.y);
	vertexes[3].m_color = tint;
	vertexes[3].m_texCoords = Vector2(textureCoordinatesAtMins.x, textureCoordinatesAtMaxs.y);

	glEnable(GL_TEXTURE_2D);
	DrawVertex2_PCT(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS, texture);
	glPopMatrix();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Texture* Renderer::CreateTextureFromFile(const std::string& imageFilePath)
{
	// Load image data
	int width = 0;
	int height = 0;
	int bytesPerTexel = 0;
	unsigned char* imageTexelBytes = stbi_load(imageFilePath.c_str(), &width, &height, &bytesPerTexel, 0);
	GUARANTEE_OR_DIE(imageTexelBytes != nullptr, Stringf("Failed to load image file \"%s\" - file not found!", imageFilePath.c_str()));
	GUARANTEE_OR_DIE(bytesPerTexel == 3 || bytesPerTexel == 4, Stringf("Failed to load image file \"%s\" - image had unsupported %i bytes per texel (must be 3 or 4)", imageFilePath.c_str(), bytesPerTexel));

	// Create texture on video card, and send image (texel) data
	unsigned int openGLTextureID = CreateOpenGLTexture(imageTexelBytes, width, height, bytesPerTexel);
	stbi_image_free(imageTexelBytes);

	// Create (new) a Texture object
	Texture* texture = new Texture();
	texture->m_textureID = openGLTextureID;
	texture->m_imageFilePath = imageFilePath;
	texture->m_texelDimensions.SetXY(width, height);

	m_alreadyLoadedTextures.push_back(texture);
	return texture;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Texture* Renderer::GetTexture(const std::string& imageFilePath)
{
	if (m_alreadyLoadedTextures.empty())
		return nullptr;

	for (int textureIndex = 0; textureIndex < (int)m_alreadyLoadedTextures.size(); ++textureIndex)
	{
		Texture* texture = m_alreadyLoadedTextures[textureIndex];
		if (imageFilePath == texture->m_imageFilePath)
			return texture;
	}

	return nullptr;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
unsigned int Renderer::CreateOpenGLTexture(unsigned char* imageTexelBytes, int width, int height, int bytesPerTexel)
{
	// Create a texture ID (or "name" as OpenGL calls it) for this new texture
	unsigned int openGLTextureID = 0xFFFFFFFF;
	glGenTextures(1, &openGLTextureID);

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture(GL_TEXTURE_2D, openGLTextureID);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // one of: GL_CLAMP or GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // one of: GL_CLAMP or GL_REPEAT

																  // Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	GLenum bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if (bytesPerTexel == 3)
		bufferFormat = GL_RGB;

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(			// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,		// Creating this as a 2d texture
		0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
		width,				// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		height,				// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,					// Border size, in texels (must be 0 or 1)
		bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		imageTexelBytes);	// Location of the actual pixel data bytes/buffer

	return openGLTextureID;
}

//*******************************************************************
SpriteSheet Renderer::CreateOrGetSpriteSheet(const std::string& imageFilePath, int tilesWide, int tilesHigh)
{
	Texture* texture = GetTexture(imageFilePath);
	if (texture)
		return SpriteSheet(texture, tilesWide, tilesHigh);

	texture = CreateTextureFromFile(imageFilePath);
	return SpriteSheet(texture, tilesWide, tilesHigh);
}

//*******************************************************************
BitmapFont* Renderer::GetFont(const std::string& bitmapFontName)
{
	if (m_loadedFonts.empty())
		return nullptr;

	BitmapFont* bitmapFont = m_loadedFonts[bitmapFontName];
	if (bitmapFont != nullptr)
		return bitmapFont;
	
	return nullptr;
}

//*******************************************************************
BitmapFont* Renderer::CreateOrGetFont(const std::string& bitmapFontName)
{
	BitmapFont* bitmapFont = GetFont(bitmapFontName);
	if (bitmapFont)
		return bitmapFont;

	bitmapFont = new BitmapFont(bitmapFontName);
	m_loadedFonts[bitmapFontName] = bitmapFont;
	return bitmapFont;
}

//*******************************************************************
void Renderer::DrawText2D(const Vector2& startBottomLeft, const std::string& asciiText, float cellHeight, const Rgba& tint, const BitmapFont* font)
{
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);	

	for (int asciiTextIndex = 0; asciiTextIndex < (int)asciiText.length(); asciiTextIndex++)
	{
		Vector2 mins = Vector2(startBottomLeft.x + (asciiTextIndex * cellHeight), startBottomLeft.y);
		Vector2 maxs = Vector2(mins.x + cellHeight, mins.y + cellHeight);
		AABB2D characterBounds = AABB2D(mins, maxs);

		char character = asciiText.at(asciiTextIndex);
		AABB2D characterTexCoords = font->GetTexCoordsForGlyph(character);
		Texture* fontTexture = font->GetTexture();
		DrawTexturedAABB2D(characterBounds, *fontTexture, characterTexCoords.m_mins, characterTexCoords.m_maxs, tint);
	}
}

//*******************************************************************
void Renderer::Translate3D(const Vector3& translation3D)
{
	glTranslatef(translation3D.x, translation3D.y, translation3D.z);
}

//*******************************************************************
void Renderer::Rotate3D(float rotationDegrees, const Vector3& rotationAxis)
{
	glRotatef(rotationDegrees, rotationAxis.x, rotationAxis.y, rotationAxis.z);
}

//*******************************************************************
void Renderer::DrawTexturedQuad3D(const Vertex3_PCT& one, const Vertex3_PCT& two, const Vertex3_PCT& three, const Vertex3_PCT& four, const Texture& texture) const
{
	const int NUM_VERTS = 4;
	Vertex3_PCT vertexes[NUM_VERTS];

	vertexes[0].m_position = one.m_position;
	vertexes[0].m_color = one.m_color;
	vertexes[0].m_texCoords = one.m_texCoords;

	vertexes[1].m_position = two.m_position;
	vertexes[1].m_color = two.m_color;
	vertexes[1].m_texCoords = two.m_texCoords;

	vertexes[2].m_position = three.m_position;
	vertexes[2].m_color = three.m_color;
	vertexes[2].m_texCoords = three.m_texCoords;

	vertexes[3].m_position = four.m_position;
	vertexes[3].m_color = four.m_color;
	vertexes[3].m_texCoords = four.m_texCoords;

	glEnable(GL_TEXTURE_2D);
	DrawVertex3_PCT(&vertexes[0], NUM_VERTS, PRIMITIVE_QUADS, texture);
}

//*******************************************************************
void Renderer::DrawVoxel(const Vector3& voxelCoordinates, const Texture& texture, const AABB2D& bottomVoxelFace, const AABB2D& sideVoxelFace, const AABB2D& topVoxelFace) const
{
	std::vector<Vertex3_PCT> vertexes;
	Rgba color(255, 255, 255);

	float x = voxelCoordinates.x;
	float y = voxelCoordinates.y;
	float z = voxelCoordinates.z;

	// bottom tile
	Vertex3_PCT one		(Vector3(x + 0.f, y + 0.f, z + 0.f), color, Vector2(0.f, 1.f));
	Vertex3_PCT two		(Vector3(x + 0.f, y + 1.f, z + 0.f), color, Vector2(1.f, 1.f));
	Vertex3_PCT three	(Vector3(x + 1.f, y + 1.f, z + 0.f), color, Vector2(1.f, 0.f));
	Vertex3_PCT four	(Vector3(x + 1.f, y + 0.f, z + 0.f), color, Vector2(0.f, 0.f));

	one.m_texCoords = bottomVoxelFace.m_mins;
	two.m_texCoords = Vector2(bottomVoxelFace.m_maxs.x, bottomVoxelFace.m_mins.y);
	three.m_texCoords = bottomVoxelFace.m_maxs;
	four.m_texCoords = Vector2(bottomVoxelFace.m_mins.x, bottomVoxelFace.m_maxs.y);

	DrawTexturedQuad3D(one, two, three, four, texture);

	// Side tiles
	one.m_texCoords = sideVoxelFace.m_mins;
	two.m_texCoords = Vector2(sideVoxelFace.m_maxs.x, sideVoxelFace.m_mins.y);
	three.m_texCoords = sideVoxelFace.m_maxs;
	four.m_texCoords = Vector2(sideVoxelFace.m_mins.x, sideVoxelFace.m_maxs.y);

	one		.m_position = Vector3(x + 0.f, y + 0.f, z + 0.f);
	two		.m_position = Vector3(x + 1.f, y + 0.f, z + 0.f);
	three	.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	four	.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	one		.m_position = Vector3(x + 1.f, y + 0.f, z + 0.f);
	two		.m_position = Vector3(x + 1.f, y + 1.f, z + 0.f);
	three	.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	four	.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	one		.m_position = Vector3(x + 0.f, y + 1.f, z + 0.f);
	two		.m_position = Vector3(x + 0.f, y + 0.f, z + 0.f);
	three	.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	four	.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	one		.m_position = Vector3(x + 1.f, y + 1.f, z + 0.f);
	two		.m_position = Vector3(x + 0.f, y + 1.f, z + 0.f);
	three	.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	four	.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	//// top tile
	one.m_texCoords = topVoxelFace.m_mins;
	two.m_texCoords = Vector2(topVoxelFace.m_maxs.x, topVoxelFace.m_mins.y);
	three.m_texCoords = topVoxelFace.m_maxs;
	four.m_texCoords = Vector2(topVoxelFace.m_mins.x, topVoxelFace.m_maxs.y);

	one		.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	two		.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	three	.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	four	.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	DrawVertexArray(&vertexes[0], vertexes.size(), PRIMITIVE_QUADS, texture);
}

//*******************************************************************
void Renderer::DrawVoxel(const Vector3& voxelCoordinates, const Texture& texture, const AABB2D& bottomVoxelFace, const AABB2D& sideVoxelFace, const AABB2D& topVoxelFace, const Rgba& tint) const
{
	std::vector<Vertex3_PCT> vertexes;
	Rgba color = tint;

	float x = voxelCoordinates.x;
	float y = voxelCoordinates.y;
	float z = voxelCoordinates.z;

	// bottom tile
	Vertex3_PCT one(Vector3(x + 0.f, y + 0.f, z + 0.f), color, Vector2(0.f, 1.f));
	Vertex3_PCT two(Vector3(x + 0.f, y + 1.f, z + 0.f), color, Vector2(1.f, 1.f));
	Vertex3_PCT three(Vector3(x + 1.f, y + 1.f, z + 0.f), color, Vector2(1.f, 0.f));
	Vertex3_PCT four(Vector3(x + 1.f, y + 0.f, z + 0.f), color, Vector2(0.f, 0.f));

	one.m_texCoords = bottomVoxelFace.m_mins;
	two.m_texCoords = Vector2(bottomVoxelFace.m_maxs.x, bottomVoxelFace.m_mins.y);
	three.m_texCoords = bottomVoxelFace.m_maxs;
	four.m_texCoords = Vector2(bottomVoxelFace.m_mins.x, bottomVoxelFace.m_maxs.y);

	DrawTexturedQuad3D(one, two, three, four, texture);

	// Side tiles
	one.m_texCoords = sideVoxelFace.m_mins;
	two.m_texCoords = Vector2(sideVoxelFace.m_maxs.x, sideVoxelFace.m_mins.y);
	three.m_texCoords = sideVoxelFace.m_maxs;
	four.m_texCoords = Vector2(sideVoxelFace.m_mins.x, sideVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 0.f, y + 0.f, z + 0.f);
	two.m_position = Vector3(x + 1.f, y + 0.f, z + 0.f);
	three.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	four.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	one.m_position = Vector3(x + 1.f, y + 0.f, z + 0.f);
	two.m_position = Vector3(x + 1.f, y + 1.f, z + 0.f);
	three.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	four.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	one.m_position = Vector3(x + 0.f, y + 1.f, z + 0.f);
	two.m_position = Vector3(x + 0.f, y + 0.f, z + 0.f);
	three.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	four.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	one.m_position = Vector3(x + 1.f, y + 1.f, z + 0.f);
	two.m_position = Vector3(x + 0.f, y + 1.f, z + 0.f);
	three.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	four.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	//// top tile
	one.m_texCoords = topVoxelFace.m_mins;
	two.m_texCoords = Vector2(topVoxelFace.m_maxs.x, topVoxelFace.m_mins.y);
	three.m_texCoords = topVoxelFace.m_maxs;
	four.m_texCoords = Vector2(topVoxelFace.m_mins.x, topVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	two.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	three.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	four.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	DrawVertexArray(&vertexes[0], vertexes.size(), PRIMITIVE_QUADS, texture);
}

//*******************************************************************
void Renderer::DrawVoxel(const Vector3& voxelCoordinates, const Texture& texture, const AABB2D& bottomVoxelFace, const AABB2D& northSideVoxelFace, const AABB2D& southSideVoxelFace, const AABB2D& eastSideVoxelFace, const AABB2D& westSideVoxelFace, const AABB2D& topVoxelFace, const Rgba& tint) const
{
	std::vector<Vertex3_PCT> vertexes;
	Rgba color = tint;

	float x = voxelCoordinates.x;
	float y = voxelCoordinates.y;
	float z = voxelCoordinates.z;

	// bottom tile
	Vertex3_PCT one(Vector3(x + 0.f, y + 0.f, z + 0.f), color, Vector2(0.f, 1.f));
	Vertex3_PCT two(Vector3(x + 0.f, y + 1.f, z + 0.f), color, Vector2(1.f, 1.f));
	Vertex3_PCT three(Vector3(x + 1.f, y + 1.f, z + 0.f), color, Vector2(1.f, 0.f));
	Vertex3_PCT four(Vector3(x + 1.f, y + 0.f, z + 0.f), color, Vector2(0.f, 0.f));

	one.m_texCoords = bottomVoxelFace.m_mins;
	two.m_texCoords = Vector2(bottomVoxelFace.m_maxs.x, bottomVoxelFace.m_mins.y);
	three.m_texCoords = bottomVoxelFace.m_maxs;
	four.m_texCoords = Vector2(bottomVoxelFace.m_mins.x, bottomVoxelFace.m_maxs.y);

	DrawTexturedQuad3D(one, two, three, four, texture);

	// Side tiles
	one.m_texCoords = southSideVoxelFace.m_mins;
	two.m_texCoords = Vector2(southSideVoxelFace.m_maxs.x, southSideVoxelFace.m_mins.y);
	three.m_texCoords = southSideVoxelFace.m_maxs;
	four.m_texCoords = Vector2(southSideVoxelFace.m_mins.x, southSideVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 0.f, y + 0.f, z + 0.f);
	two.m_position = Vector3(x + 1.f, y + 0.f, z + 0.f);
	three.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	four.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);


	one.m_texCoords = eastSideVoxelFace.m_mins;
	two.m_texCoords = Vector2(eastSideVoxelFace.m_maxs.x, eastSideVoxelFace.m_mins.y);
	three.m_texCoords = eastSideVoxelFace.m_maxs;
	four.m_texCoords = Vector2(eastSideVoxelFace.m_mins.x, eastSideVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 1.f, y + 0.f, z + 0.f);
	two.m_position = Vector3(x + 1.f, y + 1.f, z + 0.f);
	three.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	four.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);


	one.m_texCoords = westSideVoxelFace.m_mins;
	two.m_texCoords = Vector2(westSideVoxelFace.m_maxs.x, westSideVoxelFace.m_mins.y);
	three.m_texCoords = westSideVoxelFace.m_maxs;
	four.m_texCoords = Vector2(westSideVoxelFace.m_mins.x, westSideVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 0.f, y + 1.f, z + 0.f);
	two.m_position = Vector3(x + 0.f, y + 0.f, z + 0.f);
	three.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	four.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);


	one.m_texCoords = northSideVoxelFace.m_mins;
	two.m_texCoords = Vector2(northSideVoxelFace.m_maxs.x, northSideVoxelFace.m_mins.y);
	three.m_texCoords = northSideVoxelFace.m_maxs;
	four.m_texCoords = Vector2(northSideVoxelFace.m_mins.x, northSideVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 1.f, y + 1.f, z + 0.f);
	two.m_position = Vector3(x + 0.f, y + 1.f, z + 0.f);
	three.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	four.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	//// top tile
	one.m_texCoords = topVoxelFace.m_mins;
	two.m_texCoords = Vector2(topVoxelFace.m_maxs.x, topVoxelFace.m_mins.y);
	three.m_texCoords = topVoxelFace.m_maxs;
	four.m_texCoords = Vector2(topVoxelFace.m_mins.x, topVoxelFace.m_maxs.y);

	one.m_position = Vector3(x + 0.f, y + 0.f, z + 1.f);
	two.m_position = Vector3(x + 1.f, y + 0.f, z + 1.f);
	three.m_position = Vector3(x + 1.f, y + 1.f, z + 1.f);
	four.m_position = Vector3(x + 0.f, y + 1.f, z + 1.f);
	//DrawTexturedQuad3D(one, two, three, four, texture);
	vertexes.push_back(one);
	vertexes.push_back(two);
	vertexes.push_back(three);
	vertexes.push_back(four);

	DrawVertexArray(&vertexes[0], vertexes.size(), PRIMITIVE_QUADS, texture);
}


//*******************************************************************
void Renderer::PushMatrix() const
{
	glPushMatrix();
}

//*******************************************************************
void Renderer::PopMatrix() const
{
	glPopMatrix();
}

//*******************************************************************
void Renderer::Translate(float x, float y, float z) const
{
	glTranslatef(x, y, z);
}

//*******************************************************************
void Renderer::Rotate(float orientationDegrees, float x, float y, float z) const
{
	glRotatef(orientationDegrees, x, y, z);
}

//*******************************************************************
void Renderer::Scale(float x, float y, float z) const
{
	glScalef(x, y, z);
}

//*******************************************************************
void Renderer::LoadIdentity() const
{
	glLoadIdentity();
}

//*******************************************************************
void Renderer::Ortho(float left, float right, float bottom, float top, float zNear, float zFar) const
{
	glOrtho(left, right, bottom, top, zNear, zFar);
}

//*******************************************************************
void Renderer::ClearColor(float red, float green, float blue, float alpha) const
{
	glClearColor(red, green, blue, alpha);
}

//*******************************************************************
void Renderer::EnableBlending() const
{
	glEnable(GL_BLEND);
}

//*******************************************************************
void Renderer::EnableSmooth() const
{
	glEnable(GL_LINE_SMOOTH);
}

//*******************************************************************
void Renderer::EnableLighting() const
{
	glEnable(GL_LIGHTING);
}

//*******************************************************************
void Renderer::DisableLighting() const
{
	glDisable(GL_LIGHTING);
}

//*******************************************************************
void Renderer::EnableColorMaterial() const
{
	glEnable(GL_COLOR_MATERIAL);
}

//*******************************************************************
void Renderer::DisableColorMaterial() const
{
	glDisable(GL_COLOR_MATERIAL);
}

//*******************************************************************
void Renderer::ColorMaterial() const
{
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

//*******************************************************************
void Renderer::AlphaBlendFunc() const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//*******************************************************************
void Renderer::LineThickness(float thickness) const
{
	glLineWidth(thickness);
}

//*******************************************************************
void Renderer::PointSize(float size) const
{
	glPointSize(size);
}

//*******************************************************************
void Renderer::EnableDepthTesting()
{
	glEnable(GL_DEPTH_TEST);
}

//*******************************************************************
void Renderer::DisableDepthTesting()
{
	glDisable(GL_DEPTH_TEST);
}

//*******************************************************************
void Renderer::BindTexture(Texture* textureToBind)
{
	if (textureToBind != nullptr)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureToBind->m_textureID);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);//#TO_DO: bind default white texture
		Texture* whiteTexture = CreateOrGetTexture("Data/Images/WhiteTexture.png");
		glBindTexture(GL_TEXTURE_2D, whiteTexture->m_textureID);
	}
	
}

//*******************************************************************
void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
