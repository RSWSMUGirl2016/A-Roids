#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Disc2D.hpp"

class Entity
{
public:
	Vector2	m_position;
	Vector2	m_velocity;
	Vector2	m_acceleration;
	float m_spinDegreesPerSecond;
	float m_orientationDegrees;
	float m_radius;
	Disc2D m_physicalRadius;
	Disc2D m_liberalRadius;
public:
	Entity();
	~Entity();
	virtual void Update(float deltaSeconds);
	virtual void Render() const;
	virtual void WrapAround();
public:
	virtual void SetCenterForPhysicalAndLiberalRadii();
	virtual void SetRadiiForPhysicalAndLiberalRadii();
};