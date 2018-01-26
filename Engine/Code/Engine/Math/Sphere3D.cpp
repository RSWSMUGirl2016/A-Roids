#include "Engine/Math/Sphere3D.hpp"

//*******************************************************************
const Sphere3D Sphere3D::UNIT_SPHERE = Sphere3D(0.f, 0.f, 0.f, 1.f);

//*******************************************************************
Sphere3D::Sphere3D()
	: center(0.f, 0.f, 0.f)
	, radius(0.f)
{
}

//*******************************************************************
Sphere3D::~Sphere3D()
{
}

//*******************************************************************
Sphere3D::Sphere3D(const Sphere3D& copy)
	: center(copy.center)
	, radius(copy.radius)
{
}

//*******************************************************************
Sphere3D::Sphere3D(float initialX, float initialY, float initialZ, float initialRadius)
	: center(initialX, initialY, initialZ)
	, radius(initialRadius)
{
}

//*******************************************************************
Sphere3D::Sphere3D(const Vector3& initialCenter, float initialRadius)
	: center(initialCenter)
	, radius(initialRadius)
{
}

//*******************************************************************
void Sphere3D::StretchToIncludePoint(const Vector3& point)
{
	float distanceBetweenCenterAndPoint = CalcDistance(center, point);
	radius = distanceBetweenCenterAndPoint;
}

//*******************************************************************
void Sphere3D::AddPadding(float paddingRadius)
{
	radius += paddingRadius;
}

//*******************************************************************
void Sphere3D::Translate(const Vector3& tranlsation)
{
	center += tranlsation;
}

//*******************************************************************
bool Sphere3D::IsPointInside(const Vector3& point) const
{
	float distanceBetweenCenterAndPoint = CalcDistance(center, point);
	return distanceBetweenCenterAndPoint < radius;
}

//*******************************************************************
const Sphere3D Sphere3D::operator+(const Vector3& translation) const
{
	return Sphere3D(center + translation, radius);
}

//*******************************************************************
const Sphere3D Sphere3D::operator-(const Vector3& antiTranslation) const
{
	return Sphere3D(center - antiTranslation, radius);
}

//*******************************************************************
void Sphere3D::operator+=(const Vector3& translation)
{
	center += translation;
}

//*******************************************************************
void Sphere3D::operator-=(const Vector3& antiTranslation)
{
	center -= antiTranslation;
}

//*******************************************************************
const Sphere3D Interpolate(const Sphere3D& start, const Sphere3D& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	Sphere3D blended;
	blended.center = (fractionOfStart * start.center) + (fractionToEnd * end.center);
	blended.radius = (fractionOfStart * start.radius) + (fractionToEnd * end.radius);
	return blended;

}
