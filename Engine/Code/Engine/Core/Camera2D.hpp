#pragma once
#include "Engine/Math/Vector2.hpp"

class Camera2D
{
public:
	Vector2 m_worldPostition;
	float m_orientationDegrees;
	float m_numTilesInViewVertically;
public:
	Camera2D();
	~Camera2D();
};

extern Camera2D* g_theCamera2D;