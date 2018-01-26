#pragma once
#include <math.h>

class IntVector3
{
public:
	int x;
	int y;
	int z;
public:
	IntVector3();
	IntVector3(const IntVector3& copy);
	explicit IntVector3(int initialX, int initialY, int initialZ);
public:
	void GetXYZ(int& out_x, int& out_y, int& out_z) const;
	const int* GetAsFloatArray() const;
	int* GetAsFloatArray();
	int CalcLength() const;
	int CalcLengthSquared() const;
public:
	void SetXYZ(int newX, int newY, int newZ);
	int Normalize();
	int SetLength(int newLength);
	void ScaleUniform(int scale);
	void ScaleNonUniform(const IntVector3& perAxisScaleFactors);
	void InverseScaleNonUniform(const IntVector3& perAxisDivisors);
public:
	bool operator==(const IntVector3& vectorToEqual) const;
	bool operator!=(const IntVector3& vectorToNotEqual) const;
	const IntVector3 operator+(const IntVector3& vectorToAdd) const;
	const IntVector3 operator-(const IntVector3& vectorToSubtract) const;
	const IntVector3 operator*(int scale) const;
	const IntVector3 operator*(const IntVector3& perAxisScaleFactors) const;
	const IntVector3 operator/(int inverseScale) const;
	const IntVector3 operator/(const IntVector3& perAxisInverseScaleFactors) const;
	void operator*=(int scale);
	void operator*=(const IntVector3& perAxisScaleFactors);
	void operator+=(const IntVector3& vectorToAdd);
	void operator-=(const IntVector3& vectorToSubtract);
public:
	friend int CalcDistance(const IntVector3& positionA, const IntVector3& positionB);
	friend int CalcDistanceSquared(const IntVector3& positionA, const IntVector3& positionB);
	friend const IntVector3 operator*(int scale, const IntVector3& vectorToScale);
	friend int DotProduct(const IntVector3& a, const IntVector3& b);
public:
	friend const IntVector3 Interpolate(const IntVector3& start, const IntVector3& end, float fractionToEnd);
};
