#pragma once
#include "Engine/Math/Vector2.hpp"

class AABB2D {
public:
	static const AABB2D ZERO_TO_ONE;
public:
	AABB2D();
	~AABB2D();
	AABB2D(const AABB2D& copy);
	explicit AABB2D(float initialX, float initialY);
	explicit AABB2D(float minX, float minY, float maxX, float maxY);
	explicit AABB2D(const Vector2& mins, const Vector2& maxs);
	explicit AABB2D(const Vector2& center, float radiusX, float radiusY);
public:
	Vector2 m_mins;
	Vector2 m_maxs;
public:
	void StretchToIncludePoint(const Vector2& point);
	void AddPadding(float xPaddingRadius, float yPaddingRadius);
	void Translate(const Vector2& translation);
public:
	bool IsPointInside(const Vector2& point) const;
	const Vector2 CalcSize() const;
	const Vector2 CalcCenter() const;

    //Range Mapping OR Evaluate
	const Vector2 GetPointAtNormalizedPositionWithinBox(const Vector2& normalizedPosition) const;
	
    //Get UV Coordinate
    const Vector2 GetNormalizedPositionForPointWithinBox(const Vector2& point) const;

public: 
	const AABB2D operator+(const Vector2& translation) const;
	const AABB2D operator-(const Vector2& antiTranslation) const;
	void operator+=(const Vector2& translation);
	void operator-=(const Vector2& antiTranslation);
public:
	friend const AABB2D Interpolate(const AABB2D& start, const AABB2D& end, float fractionToEnd);
};