#include "Engine/Math/Disc2D.hpp"

//*******************************************************************
const Disc2D Disc2D::UNIT_CIRCLE = Disc2D(0.f, 0.f, 1.f);

//*******************************************************************
Disc2D::Disc2D()
	: center(0.f, 0.f)
	, radius(0.f)
{
}

//*******************************************************************
Disc2D::~Disc2D()
{
}

//*******************************************************************
Disc2D::Disc2D(const Disc2D& copy)
	: center(copy.center)
	, radius(copy.radius)
{
}

//*******************************************************************
Disc2D::Disc2D(float initialX, float initialY, float initialRadius)
	: center(initialX, initialY)
	, radius(initialRadius)
{
}

//*******************************************************************
Disc2D::Disc2D(const Vector2& initialCenter, float initialRadius)
	: center(initialCenter)
	, radius(initialRadius)
{
}

//*******************************************************************
void Disc2D::StretchToIncludePoint(const Vector2& point)
{
	float distanceBetweenCenterAndPoint = CalcDistance(center, point);
	radius = distanceBetweenCenterAndPoint;
}

//*******************************************************************
void Disc2D::AddPadding(float paddingRadius)
{
	radius += paddingRadius;
}

//*******************************************************************
void Disc2D::Translate(const Vector2& tranlsation)
{
	center += tranlsation;
}

//*******************************************************************
bool Disc2D::IsPointInside(const Vector2& point) const
{
	float distanceBetweenCenterAndPoint = CalcDistance(center, point);
	return distanceBetweenCenterAndPoint < radius;
}

//*******************************************************************
const Disc2D Disc2D::operator+(const Vector2& translation) const
{
	return Disc2D(center + translation, radius);
}

//*******************************************************************
const Disc2D Disc2D::operator-(const Vector2& antiTranslation) const
{
	return Disc2D(center - antiTranslation, radius);
}

//*******************************************************************
void Disc2D::operator+=(const Vector2& translation)
{
	center += translation;
}

//*******************************************************************
void Disc2D::operator-=(const Vector2& antiTranslation)
{
	center -= antiTranslation;
}

//*******************************************************************
const Disc2D Interpolate(const Disc2D& start, const Disc2D& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	Disc2D blended;
	blended.center = (fractionOfStart * start.center) + (fractionToEnd * end.center);
	blended.radius = (fractionOfStart * start.radius) + (fractionToEnd * end.radius);
	return blended;
}