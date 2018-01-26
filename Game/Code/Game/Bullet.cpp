#include "Game/Bullet.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
Bullet::Bullet()
	: m_totalDeltaSeconds(0.f)
{
	m_position = Vector2(0.f, 0.f);
	m_velocity = Vector2(0.f, 0.f);
	m_acceleration = Vector2(0.f, 0.f);
	m_spinDegreesPerSecond = 0.f;
	m_orientationDegrees = 0.f;
	m_radius = 10.f;
	m_physicalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_BULLET);
	m_liberalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_BULLET);
}

//*******************************************************************
Bullet::~Bullet()
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
void Bullet::Update(float deltaSeconds)
{
	m_totalDeltaSeconds += deltaSeconds;
	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);
	m_liberalRadius.center = m_position;
	m_physicalRadius.center = m_position;
}

//*******************************************************************
void Bullet::Render() const
{
    g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerBulletTextureCoords.m_mins, g_theGame->m_playerBulletTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
}

//*******************************************************************
void Bullet::WrapAround()
{
	if (m_liberalRadius.center.y > (ORHTO_HEIGHT + m_liberalRadius.radius))
	{
		m_liberalRadius.center.y = 0.f - m_liberalRadius.radius;
		m_position.y = m_liberalRadius.center.y;
		m_physicalRadius.center = m_position;
	}
	if (m_liberalRadius.center.y < (0.f - m_liberalRadius.radius))
	{
		m_liberalRadius.center.y = ORHTO_HEIGHT + m_liberalRadius.radius;
		m_position.y = m_liberalRadius.center.y;
		m_physicalRadius.center = m_position;
	}
	if (m_liberalRadius.center.x >(ORHTO_WIDTH + m_liberalRadius.radius))
	{
		m_liberalRadius.center.x = 0.f - m_liberalRadius.radius;
		m_position.x = m_liberalRadius.center.x;
		m_physicalRadius.center = m_position;
	}
	if (m_liberalRadius.center.x < (0.f - m_liberalRadius.radius))
	{
		m_liberalRadius.center.x = ORHTO_WIDTH + m_liberalRadius.radius;
		m_position.x = m_liberalRadius.center.x;
		m_physicalRadius.center = m_position;
	}
}

//*******************************************************************
void Bullet::SetCenterForPhysicalAndLiberalRadii()
{
	m_physicalRadius.center = m_position;
	m_liberalRadius.center = m_position;
}

//*******************************************************************
void Bullet::SetRadiiForPhysicalAndLiberalRadii()
{
	m_physicalRadius.radius = m_radius;
	m_liberalRadius.radius = m_radius;
}
