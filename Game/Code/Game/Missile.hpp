#pragma once
#include "Game/Entity.hpp"

const float PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_MISSILE = 0.80f;
const float PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_MISSILE = 1.f;

class Missile : public Entity
{
public:
	float m_totalDeltaSeconds;
public:
    Missile();
	~Missile();
	virtual void Update(float deltaSeconds);
	virtual void Render() const;
	virtual void WrapAround();
public:
	virtual void SetCenterForPhysicalAndLiberalRadii();
	virtual void SetRadiiForPhysicalAndLiberalRadii();
};