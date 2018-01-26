#include "Engine/Math/AABB2D.hpp"

//*******************************************************************
const AABB2D AABB2D::ZERO_TO_ONE = AABB2D(Vector2(0.f, 0.f), Vector2(1.f, 1.f));

//*******************************************************************
AABB2D::AABB2D()
	: m_mins(0.f, 0.f)
	, m_maxs(0.f, 0.f)
{
}

//*******************************************************************
AABB2D::~AABB2D()
{
}

AABB2D::AABB2D(const AABB2D& copy)
	: m_mins(copy.m_mins)
	, m_maxs(copy.m_maxs)
{
}

AABB2D::AABB2D(float initialX, float initialY)
	: m_mins(initialX, initialY)
	, m_maxs(initialX, initialY)
{
}

//*******************************************************************
AABB2D::AABB2D(float minX, float minY, float maxX, float maxY)
	: m_mins(minX, minY)
	, m_maxs(maxX, maxY)
{
}

//*******************************************************************
AABB2D::AABB2D(const Vector2& mins, const Vector2& maxs)
	: m_mins(mins)
	, m_maxs(maxs)
{
}

//*******************************************************************
AABB2D::AABB2D(const Vector2& center, float radiusX, float radiusY)
{
	float halfRadiusX = radiusX / 2.0f;
	float halfRadiusY = radiusY / 2.0f;

	float minX = center.x - halfRadiusX;
	float minY = center.y - halfRadiusY;
	m_mins = Vector2(minX, minY);

	float maxX = center.x + halfRadiusX;
	float maxY = center.y + halfRadiusY;
	m_maxs = Vector2(maxX, maxY);
}

//*******************************************************************
void AABB2D::StretchToIncludePoint(const Vector2& point)
{
	float distanceBetweenPointAndMins = CalcDistanceSquared(m_mins, point);
	float distanceBetweenPointAndMaxs = CalcDistanceSquared(m_maxs, point);

	if (distanceBetweenPointAndMins <= distanceBetweenPointAndMaxs)
		m_mins = point;
	else
		m_maxs = point;
}

//*******************************************************************
void AABB2D::AddPadding(float xPaddingRadius, float yPaddingRadius)
{
	m_mins = Vector2(m_mins.x - xPaddingRadius, m_mins.x - yPaddingRadius);
	m_maxs = Vector2(m_maxs.x + xPaddingRadius, m_maxs.y + yPaddingRadius);
}

//*******************************************************************
void AABB2D::Translate(const Vector2& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
bool AABB2D::IsPointInside(const Vector2& point) const
{
	if ((point.x > m_mins.x) && (point.y > m_mins.y) && (point.x < m_maxs.x) && (point.y < m_maxs.y))
		return true;
	return false;
}

//*******************************************************************
const Vector2 AABB2D::CalcSize() const
{
	float radiusX = m_maxs.x - m_mins.x;
	float radiusY = m_maxs.y - m_mins.y;
	return Vector2(radiusX, radiusY);
}

//*******************************************************************
const Vector2 AABB2D::CalcCenter() const
{
	Vector2 size = CalcSize();
	float centerX = m_mins.x + (size.x / 2.f);
	float centerY = m_mins.y + (size.y / 2.f);
	return Vector2(centerX, centerY);
}

//*******************************************************************
//Range Mapping OR Evaluate
const Vector2 AABB2D::GetPointAtNormalizedPositionWithinBox(const Vector2& normalizedPosition) const
{
	AABB2D zeroToOne = ZERO_TO_ONE;
	Vector2 position = ((normalizedPosition - zeroToOne.m_mins)/(zeroToOne.m_maxs - m_mins))*(m_maxs - m_mins) + m_mins;
	return position;
}

//*******************************************************************
//Get UV Coordinate
const Vector2 AABB2D::GetNormalizedPositionForPointWithinBox(const Vector2& point) const
{
	AABB2D zeroToOne = ZERO_TO_ONE;
	Vector2 position = ((point - m_mins) / (m_maxs - zeroToOne.m_mins))*(zeroToOne.m_maxs - zeroToOne.m_mins) + zeroToOne.m_mins;
	return position;
}

//*******************************************************************
const AABB2D AABB2D::operator+(const Vector2& translation) const
{
	return AABB2D(m_mins.x + translation.x, m_mins.y + translation.y, m_maxs.x + translation.x, m_maxs.y + translation.y);
}

//*******************************************************************
const AABB2D AABB2D::operator-(const Vector2& antiTranslation) const
{
	return AABB2D(m_mins.x - antiTranslation.x, m_mins.y - antiTranslation.y, m_maxs.x - antiTranslation.x, m_maxs.y - antiTranslation.y);
}

//*******************************************************************
void AABB2D::operator+=(const Vector2& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

//*******************************************************************
void AABB2D::operator-=(const Vector2& antiTranslation)
{
	m_mins -= antiTranslation;
	m_maxs -= antiTranslation;
}

//*******************************************************************
const AABB2D Interpolate(const AABB2D& start, const AABB2D& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	AABB2D blended;
	blended.m_mins = (fractionOfStart * start.m_mins) + (fractionToEnd * end.m_mins);
	blended.m_maxs = (fractionOfStart * start.m_maxs) + (fractionToEnd * end.m_maxs);
	return blended;

}

