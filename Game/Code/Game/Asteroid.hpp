#pragma once
#include "Game/Entity.hpp"

const float PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_ASTEROID = 0.85f;
const float PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_ASTEROID = 1.5f;

class Asteroid: public Entity
{
public:
	Asteroid();
	~Asteroid();
	virtual void Update(float deltaSeconds);
	virtual void Render() const;
	virtual void WrapAround();
public:
	const float SetRadius();
	virtual void SetCenterForPhysicalAndLiberalRadii();
	virtual void SetRadiiForPhysicalAndLiberalRadii();
};

