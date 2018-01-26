#pragma once
#include <string>
#include "Engine/Renderer/SpriteSheet.hpp"

#define UNUSED(x) (void)(x);

class BitmapFont
{
	friend class Renderer;
    friend class SimpleRenderer;
public:
	AABB2D GetTexCoordsForGlyph(int glyphUnicode) const;
	float GetGlyphAspect(int glyphUnicode) const { UNUSED(glyphUnicode); return 1.0f; };
	Texture* GetTexture();
	Texture* GetTexture() const;
    Texture2D* GetTexture2D();
    Texture2D* GetTexture2D() const;
private:
	BitmapFont(const std::string& bitmapFontName);
	SpriteSheet* m_spriteSheet;
};