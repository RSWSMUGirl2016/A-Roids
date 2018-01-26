#include "Engine/Renderer/Texture.hpp"

//*******************************************************************
// Credit to Squirrel Eiserloh

//*******************************************************************
Texture::Texture()
	: m_imageFilePath("UNKNOWN IMAGE FILE")
	, m_textureID(0)
{
}

//*******************************************************************
Texture::~Texture()
{
}

//*******************************************************************
unsigned int Texture::GetTextureID()
{
	return m_textureID;
}
