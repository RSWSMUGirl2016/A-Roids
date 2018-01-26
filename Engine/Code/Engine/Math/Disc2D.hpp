#pragma once
#include "Engine/Math/Vector2.hpp"

class Disc2D {
public:
	static const Disc2D UNIT_CIRCLE;
public:
	Vector2 center;
	float radius;
public:
	Disc2D();
	~Disc2D();
	Disc2D(const Disc2D& copy);
	explicit Disc2D(float initialX, float initialY, float initialRadius);
	explicit Disc2D(const Vector2& initialCenter, float initialRadius);
public:
	void StretchToIncludePoint(const Vector2& point);
	void AddPadding(float paddingRadius);
	void Translate(const Vector2& tranlsation);
public:
	bool IsPointInside(const Vector2& point) const;
public:
	const Disc2D operator+(const Vector2& translation) const;
	const Disc2D operator-(const Vector2& antiTranslation) const;
	void operator+=(const Vector2& translation);
	void operator-=(const Vector2& antiTranslation);
public:
	friend const Disc2D Interpolate(const Disc2D& start, const Disc2D& end, float fractionToEnd);
};