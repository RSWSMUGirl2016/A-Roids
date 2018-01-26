#include "Game/Alien.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"

//*******************************************************************
Alien::Alien()
{
	m_position = Vector2(0.f, 0.f);
	m_velocity = Vector2(0.f, 0.f);
	m_acceleration = Vector2(0.f, 0.f);
	m_spinDegreesPerSecond = 0.f;
	m_orientationDegrees = 90.f;
	m_radius = SetRadius();
	m_physicalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_ALIEN);
	m_liberalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_ALIEN);
    m_timeForThrusting = 0.1f;
    m_currentTimeForThrusting = 0.0f;
    m_displayThrusting = true;
    m_currentTimeToPlayThrusters = 0.0f;
    m_timeToPlayThrusters = 1.0f;
}

//*******************************************************************
Alien::~Alien()
{
}

//*******************************************************************
void Alien::Update( float deltaSeconds )
{
	m_orientationDegrees += (m_spinDegreesPerSecond * deltaSeconds);
	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);
	m_liberalRadius.center = m_position;
    m_physicalRadius.center = m_position;

    SetDisplayThrusting(deltaSeconds);
    PlayThrusters(deltaSeconds);
}

//*******************************************************************
void Alien::Render() const
{
    if (m_displayThrusting)
    {
        g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_alienSpriteSheet->GetTexture2D(), g_theGame->m_alienThrustingTextureCoords.m_mins, g_theGame->m_alienThrustingTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
    }
    else
    {
        g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_alienSpriteSheet->GetTexture2D(), g_theGame->m_alienTextureCoords.m_mins, g_theGame->m_alienTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
    }    
}

//*******************************************************************
void Alien::WrapAround()
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
const float Alien::SetRadius()
{
	return 50.f;
}

//*******************************************************************
void Alien::SetCenterForPhysicalAndLiberalRadii()
{
	m_physicalRadius.center = m_position;
	m_liberalRadius.center = m_position;
}

//*******************************************************************
void Alien::SetRadiiForPhysicalAndLiberalRadii()
{
	m_physicalRadius.radius = m_radius;
	m_liberalRadius.radius = m_radius;
}

//*******************************************************************
void Alien::SetDisplayThrusting(float deltaSeconds)
{
    if (m_currentTimeForThrusting >= m_timeForThrusting)
    {
        m_displayThrusting = !m_displayThrusting;
        m_currentTimeForThrusting = 0.0f;
    }
    else 
    {
        m_currentTimeForThrusting += deltaSeconds;
    }
}

//*******************************************************************
void Alien::PlayThrusters(float deltaSeconds)
{
    if (m_currentTimeToPlayThrusters == 0.0f)
    {
        g_theGame->PlaySound("Data/Audio/AlienSweep.wav");
        m_currentTimeToPlayThrusters += deltaSeconds;
    }
    else if (m_currentTimeToPlayThrusters < m_timeToPlayThrusters)
    {
        m_currentTimeToPlayThrusters += deltaSeconds;
    }
    else
    {
        m_currentTimeToPlayThrusters = 0.0f;
    }
}
