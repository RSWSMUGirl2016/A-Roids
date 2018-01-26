#pragma once
#include "Engine/Math/Vector2.hpp"

class LineSegment2D {
public:
	static const LineSegment2D ZERO_TO_ONE;
public:
	Vector2 startPosition;
	Vector2 endPosition;
public:
	LineSegment2D();
	~LineSegment2D();
	LineSegment2D(const LineSegment2D& copy);
	explicit LineSegment2D(float initialStartPositionX, float initialStartPositionY, float initialEndPositionX, float initialEndPositionY);
	explicit LineSegment2D(const Vector2& initialStartPosition, const Vector2& initialEndPosition);
public:
	void Translate(const Vector2& translation);
	float CalculateSlope();
	float CalculateDistanceFromPointToLineSegment(const Vector2 point);
	Vector2 CalculateClosestPointOnLineSegment(const Vector2 point);
public:
	const LineSegment2D operator+(const Vector2& translation) const;
	const LineSegment2D operator-(const Vector2& antiTranslation) const;
	void operator+=(const Vector2& translation);
	void operator-=(const Vector2& antiTranslation);
public:
	friend const LineSegment2D Interpolate(const LineSegment2D& start, const LineSegment2D& end, float fractionToEnd);
};