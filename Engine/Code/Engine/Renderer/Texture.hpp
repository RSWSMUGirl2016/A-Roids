#pragma once
#include <string>
#include "Engine/Math/IntVector2.hpp"

//*******************************************************************
// Credit to Squirrel Eiserloh

class Texture
{
	friend class Renderer;
private:
	Texture();
	~Texture();
public:
	unsigned int GetTextureID();
private:
	std::string			m_imageFilePath;
	unsigned int		m_textureID;
	IntVector2			m_texelDimensions;
};