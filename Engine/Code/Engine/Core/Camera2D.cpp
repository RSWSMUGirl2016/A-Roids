#include "Camera2D.hpp"

Camera2D* g_theCamera2D = new Camera2D();

//*******************************************************************
Camera2D::Camera2D()
	: m_worldPostition(Vector2())
	, m_orientationDegrees(0.f)
	, m_numTilesInViewVertically(0.f)
{
}

//*******************************************************************
Camera2D::~Camera2D()
{
}
