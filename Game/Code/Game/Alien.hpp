#pragma once
#include "Game/Entity.hpp"

const float PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_ALIEN = 0.85f;
const float PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_ALIEN = 1.5f;

class Alien: public Entity
{
private:
    float m_timeForThrusting;
    float m_currentTimeForThrusting;
    bool m_displayThrusting;
    float m_currentTimeToPlayThrusters;
    float m_timeToPlayThrusters;
public:
    Alien();
	~Alien();
	virtual void Update(float deltaSeconds);
	virtual void Render() const;
	virtual void WrapAround();
public:
	const float SetRadius();
	virtual void SetCenterForPhysicalAndLiberalRadii();
	virtual void SetRadiiForPhysicalAndLiberalRadii();
    void SetDisplayThrusting(float deltaSeconds);
    void PlayThrusters(float deltaSeconds);
};

