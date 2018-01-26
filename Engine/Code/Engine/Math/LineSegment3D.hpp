#pragma once
#include "Engine/Math/Vector3.hpp"

class LineSegment3D {
public:
	static const LineSegment3D ZERO_TO_ONE;
public:
	Vector3 startPosition;
	Vector3 endPosition;
public:
	LineSegment3D();
	~LineSegment3D();
	LineSegment3D(const LineSegment3D& copy);
	explicit LineSegment3D(float initialStartPositionX, float initialStartPositionY, float initialStartPositionZ, float initialEndPositionX, float initialEndPositionY, float initialEndPositionZ);
	explicit LineSegment3D(const Vector3& initialStartPosition, const Vector3& initialEndPosition);
public:
	void Translate(const Vector3& translation);
	float CalculateSlope();
	float CalculateDistanceFromPointToLineSegment(const Vector3 point);
	Vector3 CalculateClosestPointOnLineSegment(const Vector3 point);
public:
	const LineSegment3D operator+(const Vector3& translation) const;
	const LineSegment3D operator-(const Vector3& antiTranslation) const;
	void operator+=(const Vector3& translation);
	void operator-=(const Vector3& antiTranslation);
public:
	friend const LineSegment3D Interpolate(const LineSegment3D& start, const LineSegment3D& end, float fractionToEnd);
};