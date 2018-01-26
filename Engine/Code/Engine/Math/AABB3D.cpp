#include "Engine/Math/AABB3D.hpp"

//*******************************************************************
const AABB3D AABB3D::ZERO_TO_ONE = AABB3D(Vector3(0.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f));

//*******************************************************************
AABB3D::AABB3D()
	: m_mins(0.f, 0.f, 0.f)
	, m_maxs(0.f, 0.f, 0.f)
{
}

//*******************************************************************
AABB3D::~AABB3D()
{
}

AABB3D::AABB3D(const AABB3D& copy)
	: m_mins(copy.m_mins)
	, m_maxs(copy.m_maxs)
{
}

AABB3D::AABB3D(float initialX, float initialY, float initialZ)
	: m_mins(initialX, initialY, initialZ)
	, m_maxs(initialX, initialY, initialZ)
{
}

//*******************************************************************
AABB3D::AABB3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	: m_mins(minX, minY, minZ)
	, m_maxs(maxX, maxY, maxZ)
{
}

//*******************************************************************
AABB3D::AABB3D(const Vector3& mins, const Vector3& maxs)
	: m_mins(mins)
	, m_maxs(maxs)
{
}

//*******************************************************************
AABB3D::AABB3D(const Vector3& center, float radiusX, float radiusY, float radiusZ)
{
	float halfRadiusX = radiusX / 2.0f;
	float halfRadiusY = radiusY / 2.0f;
	float halfRadiusZ = radiusZ / 2.0f;

	float minX = center.x - halfRadiusX;
	float minY = center.y - halfRadiusY;
	float minZ = center.z - halfRadiusZ;
	m_mins = Vector3(minX, minY, minZ);

	float maxX = center.x + halfRadiusX;
	float maxY = center.y + halfRadiusY;
	float maxZ = center.z + halfRadiusZ;
	m_maxs = Vector3(maxX, maxY, maxZ);
}

//*******************************************************************
void AABB3D::StretchToIncludePoint(const Vector3& point)
{
	float distanceBetweenPointAndMins = CalcDistanceSquared(m_mins, point);
	float distanceBetweenPointAndMaxs = CalcDistanceSquared(m_maxs, point);

	if (distanceBetweenPointAndMins <= distanceBetweenPointAndMaxs)
		m_mins = point;
	else
		m_maxs = point;
}

//*******************************************************************
void AABB3D::AddPadding(float xPaddingRadius, float yPaddingRadius, float zPaddingRadius)
{
	m_mins = Vector3(m_mins.x - xPaddingRadius, m_mins.y - yPaddingRadius, m_mins.z - zPaddingRadius);
	m_maxs = Vector3(m_maxs.x + xPaddingRadius, m_maxs.y + yPaddingRadius, m_maxs.z + zPaddingRadius);
}

//*******************************************************************
void AABB3D::Translate(const Vector3& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
bool AABB3D::IsPointInside(const Vector3& point) const
{
	if ((point.x > m_mins.x) && (point.y > m_mins.y) && (point.z > m_mins.z) && (point.x < m_maxs.x) && (point.y < m_maxs.y) && (point.z < m_maxs.z))
		return true;
	return false;
}

//*******************************************************************
const Vector3 AABB3D::CalcSize() const
{
	float radiusX = m_maxs.x - m_mins.x;
	float radiusY = m_maxs.y - m_mins.y;
	float radiusZ = m_maxs.z - m_mins.z;
	return Vector3(radiusX, radiusY, radiusZ);
}

//*******************************************************************
const Vector3 AABB3D::CalcCenter() const
{
	Vector3 size = CalcSize();
	float centerX = m_mins.x + (size.x / 2.f);
	float centerY = m_mins.y + (size.y / 2.f);
	float centerZ = m_mins.z + (size.z / 2.f);
	return Vector3(centerX, centerY, centerZ);
}

//*******************************************************************
//Range Mapping
const Vector3 AABB3D::GetPointAtNormalizedPositionWithinBox(const Vector3& normalizedPosition) const
{
	AABB3D zeroToOne = ZERO_TO_ONE;
	Vector3 position = ((normalizedPosition - zeroToOne.m_mins) / (zeroToOne.m_maxs - m_mins))*(m_maxs - m_mins) + m_mins;
	return position;
}

//*******************************************************************
const Vector3 AABB3D::GetNormalizedPositionForPointWithinBox(const Vector3& point) const
{
	AABB3D zeroToOne = ZERO_TO_ONE;
	Vector3 position = ((point - m_mins) / (m_maxs - zeroToOne.m_mins))*(zeroToOne.m_maxs - zeroToOne.m_mins) + zeroToOne.m_mins;
	return position;
}

//*******************************************************************
const AABB3D AABB3D::operator+(const Vector3& translation) const
{
	return AABB3D(m_mins.x + translation.x, m_mins.y + translation.y, m_mins.z + translation.z, m_maxs.x + translation.x, m_maxs.y + translation.y, m_maxs.z + translation.z);
}

//*******************************************************************
const AABB3D AABB3D::operator-(const Vector3& antiTranslation) const
{
	return AABB3D(m_mins.x - antiTranslation.x, m_mins.y - antiTranslation.y, m_mins.z - antiTranslation.z, m_maxs.x - antiTranslation.x, m_maxs.y - antiTranslation.y, m_maxs.z - antiTranslation.z);
}

//*******************************************************************
void AABB3D::operator+=(const Vector3& translation)
{
	m_mins += translation;
	m_maxs += translation;
}

//*******************************************************************
void AABB3D::operator-=(const Vector3& antiTranslation)
{
	m_mins -= antiTranslation;
	m_maxs -= antiTranslation;
}

//*******************************************************************
const AABB3D Interpolate(const AABB3D& start, const AABB3D& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	AABB3D blended;
	blended.m_mins = (fractionOfStart * start.m_mins) + (fractionToEnd * end.m_mins);
	blended.m_maxs = (fractionOfStart * start.m_maxs) + (fractionToEnd * end.m_maxs);
	return blended;

}