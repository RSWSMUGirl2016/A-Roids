#pragma once
#include <math.h>

class Vector3
{
public:
	float x;
	float y;
	float z;
public:
	Vector3();
	Vector3(const Vector3& copy);
	explicit Vector3(float initialX, float initialY, float initialZ);
public:
	void GetXYZ(float& out_x, float& out_y, float& out_z) const;
	const float* GetAsFloatArray() const;
	float* GetAsFloatArray();
	float CalcLength() const;
	float CalcLengthSquared() const;
public:
	void SetXYZ(float newX, float newY, float newZ);
	float Normalize();
    float GetLength() const;
    Vector3 GetNormalized() const;
	float SetLength(float newLength);
	void ScaleUniform(float scale);
	void ScaleNonUniform(const Vector3& perAxisScaleFactors);
	void InverseScaleNonUniform(const Vector3& perAxisDivisors);
	float CalcHeadingYawDegrees() const;
public:
	bool operator==(const Vector3& vectorToEqual) const;
	bool operator!=(const Vector3& vectorToNotEqual) const;
	const Vector3 operator+(const Vector3& vectorToAdd) const;
	const Vector3 operator-(const Vector3& vectorToSubtract) const;
	const Vector3 operator*(float scale) const;
	const Vector3 operator*(const Vector3& perAxisScaleFactors) const;
	const Vector3 operator/(float inverseScale) const;
	const Vector3 operator/(const Vector3& perAxisInverseScaleFactors) const;
	void operator*=(float scale);
	void operator*=(const Vector3& perAxisScaleFactors);
	void operator+=(const Vector3& vectorToAdd);
	void operator-=(const Vector3& vectorToSubtract);
    const Vector3 CrossProduct(const Vector3& other) const;
public:
	friend float CalcDistance(const Vector3& positionA, const Vector3& positionB);
	friend float CalcDistanceSquared(const Vector3& positionA, const Vector3& positionB);
	friend const Vector3 operator*(float scale, const Vector3& vectorToScale);
	friend float DotProduct(const Vector3& a, const Vector3& b);
public:
	friend const Vector3 Interpolate(const Vector3& start, const Vector3& end, float fractionToEnd);
};
