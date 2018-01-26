#pragma once
#include <math.h>

class IntVector2
{
public:
	IntVector2();
	IntVector2(const IntVector2& copy);
	explicit IntVector2(int initialX, int initialY);
public:
	int x;
	int y;
public:
	void GetXY(int& out_x, int& out_y) const;
	const int* GetAsFloatArray() const;
	int* GetAsFloatArray();
	int CalcLength() const;
	int CalcLengthSquared() const;
	float CalcHeadingDegrees() const;
	float CalcHeadingRadians() const;
public:
	void SetXY(int newX, int newY);
	void Rotate90Degrees();
	void RotateNegative90Degrees();
	void RotateDegrees(float degrees);
	void RotateRadians(float radians);
	int Normalize();
	int SetLength(int newLength);
	void ScaleUniform(int scale);
	void ScaleNonUniform(const IntVector2& perAxisScaleFactors);
	void InverseScaleNonUniform(const IntVector2& perAxisDivisors);
	void SetUnitLengthAndYawDegrees(float yawDegrees);
	void SetUnitLengthAndYawRadians(float yawRadians);
	void SetLengthAndYawDegrees(int newLength, float yawDegrees);
	void SetLengthAndYawRadians(int newLength, float yawRadians);
public:
	bool operator==(const IntVector2& vectorToEqual) const;
	bool operator!=(const IntVector2& vectorToNotEqual) const;
	const IntVector2 operator+(const IntVector2& vectorToAdd) const;
	const IntVector2 operator-(const IntVector2& vectorToSubtract) const;
	const IntVector2 operator*(int scale) const;
	const IntVector2 operator*(const IntVector2& perAxisScaleFactors) const;
	const IntVector2 operator/(int inverseScale) const;
	const IntVector2 operator/(const IntVector2& perAxisInverseScaleFactors) const;
	void operator*=(int scale);
	void operator*=(const IntVector2& perAxisScaleFactors);
	void operator+=(const IntVector2& vectorToAdd);
	void operator-=(const IntVector2& vectorToSubtract);
public:
	friend int CalcDistance(const IntVector2& positionA, const IntVector2& positionB);
	friend int CalcDistanceSquared(const IntVector2& positionA, const IntVector2& positionB);
	friend const IntVector2 operator*(int scale, const IntVector2& vectorToScale);
	friend int DotProduct(const IntVector2& a, const IntVector2& b);
public:
	friend const IntVector2 Interpolate(const IntVector2& start, const IntVector2& end, float fractionToEnd);
};
