#include "Game/Starburst.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
Starburst::Starburst()
{
	m_position = Vector2(0.f, 0.f);
	m_velocity = Vector2(0.f, 0.f);
	m_acceleration = Vector2(0.f, 0.f);
	m_spinDegreesPerSecond = 0.f;
	m_orientationDegrees = 0.f;
	m_radius = 15.f;
	m_physicalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_STARBURST);
	m_liberalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_STARBURST);

	for (int vertexNum = 0; vertexNum < NUM_VERTEXES_STARBURST; vertexNum+=2)
	{
		float paddingForPosition = 0.5;
		m_vertexes[vertexNum] = Vector2(paddingForPosition, paddingForPosition);
	}
}

//*******************************************************************
Starburst::~Starburst()
{
}

//*******************************************************************
void Starburst::Update(float deltaSeconds)
{
	m_orientationDegrees += (m_spinDegreesPerSecond * deltaSeconds);
	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);
	m_liberalRadius.center = m_position;
	m_physicalRadius.center = m_position;
}

//*******************************************************************
void Starburst::Render() const
{
    g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_starbustTextureCoords.m_mins, g_theGame->m_starbustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
}

//*******************************************************************
void Starburst::WrapAround()
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
	if (m_liberalRadius.center.x > (ORHTO_WIDTH + m_liberalRadius.radius))
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
void Starburst::SetCenterForPhysicalAndLiberalRadii()
{
	m_physicalRadius.center = m_position;
	m_liberalRadius.center = m_position;
}

//*******************************************************************
void Starburst::SetRadiiForPhysicalAndLiberalRadii()
{
	m_physicalRadius.radius = m_radius;
	m_liberalRadius.radius = m_radius;
}
