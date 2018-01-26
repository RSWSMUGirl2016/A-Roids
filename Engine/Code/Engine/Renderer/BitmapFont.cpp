#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);

//*******************************************************************
AABB2D BitmapFont::GetTexCoordsForGlyph(int glyphUnicode) const
{
	return m_spriteSheet->GetTexCoordsForSpriteIndex(glyphUnicode);
}

//*******************************************************************
Texture* BitmapFont::GetTexture()
{
	return m_spriteSheet->GetTexture();
}

//*******************************************************************
Texture* BitmapFont::GetTexture() const
{
	return m_spriteSheet->GetTexture();
}

//*******************************************************************
Texture2D* BitmapFont::GetTexture2D()
{
    return m_spriteSheet->GetTexture2D();
}

//*******************************************************************
Texture2D* BitmapFont::GetTexture2D() const
{
    return m_spriteSheet->GetTexture2D();
}

//*******************************************************************
BitmapFont::BitmapFont(const std::string& bitmapFontName)
{
	UNUSED(bitmapFontName);
    if (g_theSimpleRenderer != nullptr)
    {
        m_spriteSheet = g_theSimpleRenderer->CreateSpriteSheetPointer(bitmapFontName, 16, 16);
    }
    else
    {
        m_spriteSheet = new SpriteSheet(g_theRenderer->CreateOrGetTexture(bitmapFontName), 16, 16);
    }
	//m_spriteSheet = new SpriteSheet(g_theRenderer->CreateOrGetTexture(bitmapFontName), 16, 16);
}
