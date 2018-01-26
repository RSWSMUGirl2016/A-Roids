#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

//*******************************************************************
SpriteSheet::SpriteSheet(Texture* texture, int tilesWide, int tilesHigh)
	: m_spriteSheetTexture(texture)
    , m_spriteSheetTexture2D(nullptr)
	, m_spriteLayout(tilesWide, tilesHigh)
{
}

//*******************************************************************
SpriteSheet::SpriteSheet(Texture2D* texture, int tilesWide, int tilesHigh)
    : m_spriteSheetTexture(nullptr)
    , m_spriteSheetTexture2D(texture)
    , m_spriteLayout(tilesWide, tilesHigh)
{
}

//*******************************************************************
AABB2D SpriteSheet::GetTexCoordsForSpriteCoords(int spriteX, int spriteY) const
{
	float x = (float)spriteX;
	float y = (float)spriteY;
	float layoutWidth = (float)m_spriteLayout.x;
	float layoutHeight = (float)m_spriteLayout.y;

	Vector2 maximumBounds = Vector2(y + 1.f, x);
	Vector2 minimumBounds = maximumBounds + Vector2(-1.f, 1.f);

	if (maximumBounds.x == 0.f)
		maximumBounds.x = 0.f;
	else
		maximumBounds.x /= layoutHeight;

	if (maximumBounds.y == 0.f)
		maximumBounds.y = 0.f;
	else
		maximumBounds.y /= layoutWidth;


	if (minimumBounds.x == 0.f)
		minimumBounds.x = 0.f;
	else
		minimumBounds.x /= layoutHeight;

	if (minimumBounds.y == 0.f)
		minimumBounds.y = 0.f;
	else
		minimumBounds.y /= layoutWidth;

	return AABB2D(minimumBounds, maximumBounds);
}

//*******************************************************************
AABB2D SpriteSheet::GetTexCoordsForSpriteIndex(int spriteIndex) const
{
	int spriteSheetWidth = m_spriteLayout.y;
	
	int row = spriteIndex / spriteSheetWidth;
	int column = spriteIndex % spriteSheetWidth;
	
	return GetTexCoordsForSpriteCoords(row, column);
}

//*******************************************************************
int SpriteSheet::GetNumSprites() const
{
	return m_spriteLayout.x * m_spriteLayout.y;
}

//*******************************************************************
Texture* SpriteSheet::GetTexture()
{
	return m_spriteSheetTexture;
}

//*******************************************************************
Texture* SpriteSheet::GetTexture() const
{
	return m_spriteSheetTexture;
}

//*******************************************************************
Texture2D* SpriteSheet::GetTexture2D()
{
    return m_spriteSheetTexture2D;
}

//*******************************************************************
Texture2D* SpriteSheet::GetTexture2D() const
{
    return m_spriteSheetTexture2D;
}

//*******************************************************************
IntVector2 SpriteSheet::GetLayout()
{
	return m_spriteLayout;
}

