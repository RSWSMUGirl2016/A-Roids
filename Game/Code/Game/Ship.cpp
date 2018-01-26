#include "Game/Ship.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Engine/Audio/Audio.hpp"

const float SHIP_ACCELERATION = 178.f;

//*******************************************************************
Ship::Ship()
	: m_directionUp(false)
	, m_directionLeft(false)
	, m_directionRight(false)
    , m_isSmallThrusting(false)
    , m_timeBetweenThrustFlames(0.1f)
    , m_currentTimeBetweenThrustFlames(0.0f)
    , m_timeToPlayThrusters(1.0f)
    , m_currentTimeToPlayThrusters(0.0f)
{
	m_position = Vector2(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f);
	m_velocity = Vector2(0.f, 0.f);
	m_acceleration = Vector2(0.f, 0.f);
	m_spinDegreesPerSecond = 180.f;
	m_orientationDegrees = 90.f;
	m_radius = 25.f;
	m_physicalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_SHIP);
	m_liberalRadius = Disc2D(m_position, m_radius*PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_SHIP);
}

//*******************************************************************
Ship::~Ship()
{
}

//*******************************************************************
void Ship::Update(float deltaSeconds)
{		
    if (m_acceleration == Vector2())
        m_currentTimeToPlayThrusters = 0.0f;

	if (m_directionLeft)
    {
        m_orientationDegrees += (m_spinDegreesPerSecond * deltaSeconds);
    }
    if (m_directionRight)
    {
        m_orientationDegrees -= (m_spinDegreesPerSecond * deltaSeconds);
    }
	m_velocity.x += (m_acceleration.x * deltaSeconds);
	m_velocity.y += (m_acceleration.y * deltaSeconds);
	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);
	SetCenterForPhysicalAndLiberalRadii();

    if ((m_acceleration != Vector2(0.f, 0.f)) && (m_currentTimeBetweenThrustFlames >= m_timeBetweenThrustFlames))
    {
        m_isSmallThrusting = !m_isSmallThrusting;
        m_currentTimeBetweenThrustFlames = 0.0f;
    }
    else if (m_acceleration != Vector2(0.f, 0.f))
    {
        m_currentTimeBetweenThrustFlames += deltaSeconds;
    }

    if ((m_acceleration != Vector2(0.f, 0.f)) && (m_currentTimeToPlayThrusters == 0.0f))
    {
        g_theGame->PlaySound("Data/Audio/ShipThrusters.wav");
        m_currentTimeToPlayThrusters += deltaSeconds;
    }
    else if ((m_acceleration != Vector2(0.f, 0.f)) && (m_currentTimeToPlayThrusters < m_timeToPlayThrusters))
    {
        m_currentTimeToPlayThrusters += deltaSeconds;
    }
    else /*if (m_acceleration == Vector2(0.f, 0.f))*/
    {
        m_currentTimeToPlayThrusters = 0.0f;
    }
}

//*******************************************************************
void Ship::Render() const
{
    if (m_acceleration != Vector2(0.f, 0.f))
    {
        if (m_directionLeft)
        {
            if (m_isSmallThrusting)
            {
                g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTurnLeftSmallThrustTextureCoords.m_mins, g_theGame->m_playerShipTurnLeftSmallThrustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
            }
            else
            {
                g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTurnLeftFullThrustTextureCoords.m_mins, g_theGame->m_playerShipTurnLeftFullThrustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
            }
        }
        else if (m_directionRight)
        {
            if (m_isSmallThrusting)
            {
                g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTurnRightSmallThrustTextureCoords.m_mins, g_theGame->m_playerShipTurnRightSmallThrustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
            }
            else
            {
                g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTurnRightFullThrustTextureCoords.m_mins, g_theGame->m_playerShipTurnRightFullThrustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
            }
        }
        else
        {
            if (m_isSmallThrusting)
            {
                g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipSmallThrustTextureCoords.m_mins, g_theGame->m_playerShipSmallThrustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
            }
            else
            {
                g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipFullThrustTextureCoords.m_mins, g_theGame->m_playerShipFullThrustTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
            }
        }
        return;
    }


    if (m_directionLeft)
    {
        g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTurnLeftTextureCoords.m_mins, g_theGame->m_playerShipTurnLeftTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
    }
    else if (m_directionRight)
    {
        g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTurnRightTextureCoords.m_mins, g_theGame->m_playerShipTurnRightTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
    }
    else
    {
        g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTextureCoords.m_mins, g_theGame->m_playerShipTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
    }

}

//*******************************************************************
void Ship::RenderPlayerLife() const
{
    g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTextureCoords.m_mins, g_theGame->m_playerShipTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::WHITE);
}

//*******************************************************************
void Ship::RenderHealthPickup() const
{
    g_theSimpleRenderer->DrawSprite2DCentered(g_theGame->m_spaceShipsSpriteSheet->GetTexture2D(), g_theGame->m_playerShipTextureCoords.m_mins, g_theGame->m_playerShipTextureCoords.m_maxs, m_position, m_orientationDegrees, m_radius * 2.0f, m_radius * 2.0f, Rgba::RED);
}

//*******************************************************************
void Ship::WrapAround()
{
	if (m_liberalRadius.center.y > (ORHTO_HEIGHT + m_liberalRadius.radius))
	{
		m_position.y = 0.f - m_liberalRadius.radius;
	}
	if (m_liberalRadius.center.y < (0.f - m_liberalRadius.radius))
	{
		m_position.y = ORHTO_HEIGHT + m_liberalRadius.radius;
	}
	if (m_liberalRadius.center.x > (ORHTO_WIDTH + m_liberalRadius.radius))
	{
		m_position.x = 0.f - m_liberalRadius.radius;
	}
	if (m_liberalRadius.center.x < (0.f - m_liberalRadius.radius))
	{
		m_position.x = ORHTO_WIDTH + m_liberalRadius.radius;
	}
	SetCenterForPhysicalAndLiberalRadii();
}

//*******************************************************************
void Ship::SetCenterForPhysicalAndLiberalRadii()
{
	m_physicalRadius.center = m_position;
	m_liberalRadius.center = m_position;	
}

//*******************************************************************
void Ship::CalculateThrustXboxController(float leftJoyStickR, float leftJoyStickAngle)
{
	m_acceleration.x = SHIP_ACCELERATION * leftJoyStickR * CosDegrees(leftJoyStickAngle);
	m_acceleration.y = SHIP_ACCELERATION * leftJoyStickR * SinDegrees(leftJoyStickAngle);
}

//*******************************************************************
void Ship::CalculateThrustKeyboard()
{
	float x = m_radius*CosDegrees(m_orientationDegrees);
	float y = m_radius*SinDegrees(m_orientationDegrees);
	Vector2 acceleration = Vector2(x, y);
	acceleration.Normalize();
	m_acceleration = acceleration*SHIP_ACCELERATION;
}

//*******************************************************************
void Ship::SetRadiiForPhysicalAndLiberalRadii()
{
    m_physicalRadius.radius = m_radius;
    m_liberalRadius.radius = m_radius;
}

