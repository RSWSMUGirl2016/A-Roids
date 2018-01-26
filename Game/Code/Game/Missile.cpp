#include "Game/Missile.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
Missile::Missile()
	: m_totalDeltaSeconds(0.f)
{
	m_position = Vector2(0.f, 0.f);
	m_velocity = Vector2(0.f, 0.f);
	m_acceleration = Vector2(0.f, 0.f);
	m_spinDegreesPerSecond = 0.f;
	m_orientationDegrees = 0.f;
	m_radius = 30.f;
	m_physicalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_MISSILE);
	m_liberalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_MISSILE);
}

//*******************************************************************
Missile::~Missile()
{
}

//*******************************************************************
void Missile::Update(float deltaSeconds)
{
    m_totalDeltaSeconds += (deltaSeconds * 0.5F);

    Vector2 playerPosition = g_theGame->m_ship->m_position;
    Vector2 directionVector = playerPosition - m_position;

    float angle = Atan2Degrees(directionVector.y, directionVector.x);
    m_orientationDegrees = (float)((360 + (int)angle) % 360);

    float x = m_radius*CosDegrees(m_orientationDegrees);
    float y = m_radius*SinDegrees(m_orientationDegrees);
    Vector2 velocity = Vector2(x, y);
    velocity.Normalize();
    m_velocity = velocity * MISSILE_VELOCITY;

    m_position.x += (m_velocity.x * deltaSeconds);
    m_position.y += (m_velocity.y * deltaSeconds);
    m_liberalRadius.center = m_position;
    m_physicalRadius.center = m_position;
}

//*******************************************************************
void Missile::Render() const
{
    g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_enemyMissileTextureCoords.m_mins, g_theGame->m_enemyMissileTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
}

//*******************************************************************
void Missile::WrapAround()
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
void Missile::SetCenterForPhysicalAndLiberalRadii()
{
	m_physicalRadius.center = m_position;
	m_liberalRadius.center = m_position;
}

//*******************************************************************
void Missile::SetRadiiForPhysicalAndLiberalRadii()
{
	m_physicalRadius.radius = m_radius;
	m_liberalRadius.radius = m_radius;
}
