#include "LineSegment2D.hpp"

//*******************************************************************
const LineSegment2D LineSegment2D::ZERO_TO_ONE = LineSegment2D(0.f, 0.f, 1.f, 1.f);

//*******************************************************************
LineSegment2D::LineSegment2D()
	: startPosition(0.f, 0.f)
	, endPosition(0.f, 0.f)
{
}

//*******************************************************************
LineSegment2D::~LineSegment2D()
{
}

//*******************************************************************
LineSegment2D::LineSegment2D(const LineSegment2D& copy)
	: startPosition(copy.startPosition)
	, endPosition(copy.endPosition)
{
}

//*******************************************************************
LineSegment2D::LineSegment2D(float initialStartPositionX, float initialStartPositionY, float initialEndPositionX, float initialEndPositionY)
	: startPosition(initialStartPositionX, initialStartPositionY)
	, endPosition(initialEndPositionX, initialEndPositionY)
{
}

//*******************************************************************
LineSegment2D::LineSegment2D(const Vector2& initialStartPosition, const Vector2& initialEndPosition)
	: startPosition(initialStartPosition)
	, endPosition(initialEndPosition)
{
}

//*******************************************************************
void LineSegment2D::Translate(const Vector2& translation)
{
	startPosition += translation;
	endPosition += translation;
}

//*******************************************************************
float LineSegment2D::CalculateSlope() {
	return (float)(endPosition.y - startPosition.y) / (endPosition.x - endPosition.x);
}

//*******************************************************************
float LineSegment2D::CalculateDistanceFromPointToLineSegment(const Vector2 point)
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
		Vector2 direction = endPosition - startPosition;
		direction.Normalize();
		Vector2 distanceFromStartPositionToPoint = point - startPosition;
		Vector2 pointOnLineSegment = startPosition + (direction * DotProduct(distanceFromStartPositionToPoint, direction));
		return CalcDistance(point, pointOnLineSegment);
	}
}

//*******************************************************************
Vector2 LineSegment2D::CalculateClosestPointOnLineSegment(const Vector2 point)
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
		Vector2 direction = endPosition - startPosition;
		direction.Normalize();
		Vector2 distanceFromStartPositionToPoint = point - startPosition;
		Vector2 pointOnLineSegment = startPosition + (direction * DotProduct(distanceFromStartPositionToPoint, direction));
		return pointOnLineSegment;
	}
}

//*******************************************************************
const LineSegment2D LineSegment2D::operator+(const Vector2& translation) const
{
	return LineSegment2D(startPosition + translation, endPosition + translation);
}

//*******************************************************************
const LineSegment2D LineSegment2D::operator-(const Vector2& antiTranslation) const
{
	return LineSegment2D(startPosition - antiTranslation, endPosition + antiTranslation);
}

//*******************************************************************
void LineSegment2D::operator+=(const Vector2& translation)
{
	startPosition += translation;
	endPosition += translation;
}

//*******************************************************************
void LineSegment2D::operator-=(const Vector2& antiTranslation)
{
	startPosition -= antiTranslation;
	endPosition -= antiTranslation;
}

//*******************************************************************
const LineSegment2D Interpolate(const LineSegment2D& start, const LineSegment2D& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	LineSegment2D blended;
	blended.startPosition = (fractionOfStart * start.startPosition) + (fractionToEnd * end.startPosition);
	blended.endPosition = (fractionOfStart * start.endPosition) + (fractionToEnd * end.endPosition);
	return blended;
}