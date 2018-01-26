#pragma once
#include <string>
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/RHI/Texture2D.hpp"

class SpriteSheet
{
public:
	SpriteSheet(Texture* texture, int tilesWide, int tilesHigh);
    SpriteSheet(Texture2D* texture, int tilesWide, int tilesHigh);
	AABB2D GetTexCoordsForSpriteCoords(int spriteX, int spriteY) const;
	AABB2D GetTexCoordsForSpriteIndex(int spriteIndex) const;
	int GetNumSprites() const;
	Texture* GetTexture();
    Texture2D* GetTexture2D();
	Texture* GetTexture() const;
    Texture2D* GetTexture2D() const;
	IntVector2 GetLayout();
private:
	Texture* m_spriteSheetTexture;
    Texture2D* m_spriteSheetTexture2D;
	IntVector2 m_spriteLayout;
};