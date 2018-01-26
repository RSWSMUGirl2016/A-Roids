#pragma once
#include "Engine/Math/IntVector2.hpp"

class IntAABB2D {
public:
	static const IntAABB2D ZERO_TO_ONE;
public:
	IntAABB2D();
	~IntAABB2D();
	IntAABB2D(const IntAABB2D& copy);
	explicit IntAABB2D(int initialX, int initialY);
	explicit IntAABB2D(int minX, int minY, int maxX, int maxY);
	explicit IntAABB2D(const IntVector2& mins, const IntVector2& maxs);
	explicit IntAABB2D(const IntVector2& center, int radiusX, int radiusY);
public:
	IntVector2 mins;
	IntVector2 maxs;
public:
	void StretchToIncludePoint(const IntVector2& point);
	void AddPadding(int xPaddingRadius, int yPaddingRadius);
	void Translate(const IntVector2& translation);
public:
	bool IsPointInside(const IntVector2& point) const;
	const IntVector2 CalcSize() const;
	const IntVector2 CalcCenter() const;
	const IntVector2 GetPointAtNormalizedPositionWithinBox(const IntVector2& normalizedPosition) const;
	const IntVector2 GetNormalizedPositionForPointWithinBox(const IntVector2& point) const;
public:
	const IntAABB2D operator+(const IntVector2& translation) const;
	const IntAABB2D operator-(const IntVector2& antiTranslation) const;
	void operator+=(const IntVector2& translation);
	void operator-=(const IntVector2& antiTranslation);
};