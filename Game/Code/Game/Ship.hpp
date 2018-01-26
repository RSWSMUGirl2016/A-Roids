#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/StringUtils.hpp"

const float PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_SHIP = 0.95f;
const float PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_SHIP = 1.f;

class Ship : public Entity 
{
public:
	bool m_directionUp;
	bool m_directionLeft;
	bool m_directionRight;
private:
    float m_timeBetweenThrustFlames;
    float m_currentTimeBetweenThrustFlames;
    bool m_isSmallThrusting;
    float m_timeToPlayThrusters;
    float m_currentTimeToPlayThrusters;
public:
	Ship();
	~Ship();
	virtual void Update(float deltaSeconds);
	virtual void Render() const;
    void RenderPlayerLife() const;
    virtual void RenderHealthPickup() const;
	virtual void WrapAround();
public:
	virtual void SetCenterForPhysicalAndLiberalRadii();
	void CalculateThrustXboxController(float leftJoyStickR, float leftJoyStickAngle);
	void CalculateThrustKeyboard();
	virtual void SetRadiiForPhysicalAndLiberalRadii();
};