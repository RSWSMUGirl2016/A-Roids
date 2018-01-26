#include "Engine/Renderer/Renderer.hpp"
#include "Game/Explosion.hpp"
#include "Game/GameCommon.hpp"

//*******************************************************************
Explosion::Explosion(const SpriteAnimation& spriteAnimation, Vector2 position, float radius)
	: m_spriteAnimation(spriteAnimation)
	, m_radius(radius)
	, m_position(position)
{
}

//*******************************************************************
Explosion::~Explosion()
{
}

//*******************************************************************
void Explosion::Update(float deltaSeconds)
{
	m_spriteAnimation.Update(deltaSeconds);
}

//*******************************************************************
void Explosion::Render() const
{
	AABB2D textureBounds = m_spriteAnimation.GetCurrentTextureCoordinates();
	AABB2D bounds(m_position, m_radius, m_radius);
	
	//DrawAdditiveBlendedTexture(const AABB2D& bounds, float orientationDegrees, float scale, const Texture& texture, const Vector2& textureCoordinatesAtMins, const Vector2& textureCoordinatesAtMaxs, const Rgba& tint)
	
	//renderer, set additive blending
	//g_theRenderer->SetAdditivieBlending();
	//call regular renderer draw textured AABB2D
	//g_theRenderer->DrawTexturedAABB2D(bounds, 0.f, 1.f, *m_spriteAnimation.GetTexture(), textureBounds.m_mins, textureBounds.m_maxs, Rgba(1.f, 1.f, 1.f, 1.f));
	//renderer, set alpha blending
	//g_theRenderer->SetAlphaBlending();

	//g_theRenderer->DrawAdditiveBlendedTexture(bounds, m_scale, 1.f, *m_spriteAnimation.GetTexture(), bounds.m_mins, bounds.m_maxs, Rgba(1.f, 1.f, 1.f, 1.f));
}

//*******************************************************************
void Explosion::RenderDX() const
{
    AABB2D textureBounds = m_spriteAnimation.GetCurrentTextureCoordinates();
    AABB2D properTextureBounds = AABB2D();
    properTextureBounds.m_mins = Vector2(textureBounds.m_maxs.y, textureBounds.m_maxs.x);
    properTextureBounds.m_maxs = Vector2(textureBounds.m_mins.y, textureBounds.m_mins.x);

    AABB2D bounds(m_position, m_radius, m_radius);
    
    g_theSimpleRenderer->EnableBlend(BLEND_SRC_ALPHA, BLEND_INV_SRC_ALPHA);
    g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_explosionSpriteSheet->GetTexture2D(), properTextureBounds.m_mins, properTextureBounds.m_maxs, m_position, 0.0f, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
    g_theSimpleRenderer->DisableBlend();
}

