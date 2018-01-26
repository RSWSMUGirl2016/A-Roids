#pragma once
#include "Game/Entity.hpp"

const int NUM_VERTEXES_STARBURST = 24;
const float PERECENT_OF_M_RADIUS_FOR_PHYSICAL_RADIUS_FOR_STARBURST = 0.95f;
const float PERECENT_OF_M_RADIUS_FOR_LIBERAL_RADIUS_FOR_STARBURST = 1.f;

class Starburst : public Entity
{
public:
	Vector2 m_vertexes[NUM_VERTEXES_STARBURST];
public:
	Starburst();
	~Starburst();
	virtual void Update(float deltaSeconds);
	virtual void Render() const;
	virtual void WrapAround();
public:
	virtual void SetCenterForPhysicalAndLiberalRadii();
	virtual void SetRadiiForPhysicalAndLiberalRadii();
};