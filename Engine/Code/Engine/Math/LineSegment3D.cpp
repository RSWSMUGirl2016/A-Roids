#include "LineSegment3D.hpp"

//*******************************************************************
const LineSegment3D LineSegment3D::ZERO_TO_ONE = LineSegment3D(0.f, 0.f, 0.f, 1.f, 1.f, 1.f);

//*******************************************************************
LineSegment3D::LineSegment3D()
	: startPosition(0.f, 0.f, 0.f)
	, endPosition(0.f, 0.f, 0.f)
{
}

//*******************************************************************
LineSegment3D::~LineSegment3D()
{
}

//*******************************************************************
LineSegment3D::LineSegment3D(const LineSegment3D& copy)
	: startPosition(copy.startPosition)
	, endPosition(copy.endPosition)
{
}

//*******************************************************************
LineSegment3D::LineSegment3D(float initialStartPositionX, float initialStartPositionY, float initialStartPositionZ, float initialEndPositionX, float initialEndPositionY, float initialEndPositionZ)
	: startPosition(initialStartPositionX, initialStartPositionY, initialStartPositionZ)
	, endPosition(initialEndPositionX, initialEndPositionY, initialEndPositionZ)
{
}

//*******************************************************************
LineSegment3D::LineSegment3D(const Vector3& initialStartPosition, const Vector3& initialEndPosition)
	: startPosition(initialStartPosition)
	, endPosition(initialEndPosition)
{
}

//*******************************************************************
void LineSegment3D::Translate(const Vector3& translation)
{
	startPosition += translation;
	endPosition += translation;
}

//*******************************************************************
float LineSegment3D::CalculateSlope() {
	Vector3 endPointXY = Vector3(endPosition.x, endPosition.y, startPosition.z);
	float run = CalcDistance(startPosition, endPointXY);
	float rise = CalcDistance(endPointXY, endPosition);
	return rise / run;
}

//*******************************************************************
float LineSegment3D::CalculateDistanceFromPointToLineSegment(const Vector3 point)
{
	if (DotProduct(point - startPosition, endPosition - startPosition) < 0.f)
	{
		return CalcDistance(point, startPosition);
	}
	else if (DotProduct(point - endPosition, endPosition - startPosition) > 0.f)
	{
		return CalcDistance(point, endPosition);
	}
	else
	{
		Vector3 direction = endPosition - startPosition;
		direction.Normalize();
		Vector3 distanceFromStartPositionToPoint = point - startPosition;
		Vector3 pointOnLineSegment = startPosition + (direction * DotProduct(distanceFromStartPositionToPoint, direction));
		return CalcDistance(point, pointOnLineSegment);
	}
}

//*******************************************************************
Vector3 LineSegment3D::CalculateClosestPointOnLineSegment(const Vector3 point)
{
	if (DotProduct(point - startPosition, endPosition - startPosition) < 0.f)
	{
		return startPosition;
	}
	else if (DotProduct(point - endPosition, endPosition - startPosition) > 0.f)
	{
		return endPosition;
	}
	else
	{
		Vector3 direction = endPosition - startPosition;
		direction.Normalize();
		Vector3 distanceFromStartPositionToPoint = point - startPosition;
		Vector3 pointOnLineSegment = startPosition + (direction * DotProduct(distanceFromStartPositionToPoint, direction));
		return pointOnLineSegment;
	}
}

//*******************************************************************
const LineSegment3D LineSegment3D::operator+(const Vector3& translation) const
{
	return LineSegment3D(startPosition + translation, endPosition + translation);
}

//*******************************************************************
const LineSegment3D LineSegment3D::operator-(const Vector3& antiTranslation) const
{
	return LineSegment3D(startPosition - antiTranslation, endPosition + antiTranslation);
}

//*******************************************************************
void LineSegment3D::operator+=(const Vector3& translation)
{
	startPosition += translation;
	endPosition += translation;
}

//*******************************************************************
void LineSegment3D::operator-=(const Vector3& antiTranslation)
{
	startPosition -= antiTranslation;
	endPosition -= antiTranslation;
}

//*******************************************************************
const LineSegment3D Interpolate(const LineSegment3D& start, const LineSegment3D& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	LineSegment3D blended;
	blended.startPosition = (fractionOfStart * start.startPosition) + (fractionToEnd * end.startPosition);
	blended.endPosition = (fractionOfStart * start.endPosition) + (fractionToEnd * end.endPosition);
	return blended;
}