#pragma once
#include <math.h>

class UnsignedIntVector4
{
public:
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;
public:
	UnsignedIntVector4();
	UnsignedIntVector4(const UnsignedIntVector4& copy);
	explicit UnsignedIntVector4(unsigned int initialX, unsigned int initialY, unsigned int initialZ, unsigned int initialW);
public:
	void GetXYZW(unsigned int& out_x, unsigned int& out_y, unsigned int& out_z, unsigned int& out_w) const;
	const unsigned int* GetAsUnsignedIntArray() const;
	unsigned int* GetAsUnsignedIntArray();
	unsigned int CalcLength() const;
	unsigned int CalcLengthSquared() const;
public:
	void SetXYZW(unsigned int newX, unsigned int newY, unsigned int newZ, unsigned int newW);
	unsigned int Normalize3D();
	unsigned int Normalize4D();
	unsigned int SetLength(unsigned int newLength);
	void ScaleUniform(unsigned int scale);
	void ScaleNonUniform(const UnsignedIntVector4& perAxisScaleFactors);
	void InverseScaleNonUniform(const UnsignedIntVector4& perAxisDivisors);
public:
	bool operator==(const UnsignedIntVector4& vectorToEqual) const;
	bool operator!=(const UnsignedIntVector4& vectorToNotEqual) const;
	const UnsignedIntVector4 operator+(const UnsignedIntVector4& vectorToAdd) const;
	const UnsignedIntVector4 operator-(const UnsignedIntVector4& vectorToSubtract) const;
	const UnsignedIntVector4 operator*(unsigned int scale) const;
	const UnsignedIntVector4 operator*(const UnsignedIntVector4& perAxisScaleFactors) const;
	const UnsignedIntVector4 operator/(unsigned int inverseScale) const;
	const UnsignedIntVector4 operator/(const UnsignedIntVector4& perAxisInverseScaleFactors) const;
	void operator*=(unsigned int scale);
	void operator*=(const UnsignedIntVector4& perAxisScaleFactors);
	void operator+=(const UnsignedIntVector4& vectorToAdd);
	void operator-=(const UnsignedIntVector4& vectorToSubtract);
public:
	friend unsigned int CalcDistance(const UnsignedIntVector4& positionA, const UnsignedIntVector4& positionB);
	friend unsigned int CalcDistanceSquared(const UnsignedIntVector4& positionA, const UnsignedIntVector4& positionB);
	friend const UnsignedIntVector4 operator*(unsigned int scale, const UnsignedIntVector4& vectorToScale);
	friend unsigned int DotProduct(const UnsignedIntVector4& a, const UnsignedIntVector4& b);
public:
	friend const UnsignedIntVector4 Interpolate(const UnsignedIntVector4& start, const UnsignedIntVector4& end, unsigned int fractionToEnd);
};
