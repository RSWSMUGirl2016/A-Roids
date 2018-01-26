#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/UnsignedIntVector4.hpp"
#include <iostream>

using namespace std;

//*******************************************************************
// Credit to Squirrel Eiserloh
UnsignedIntVector4::UnsignedIntVector4()
	: x(0)
	, y(0)
	, z(0)
	, w(0)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
UnsignedIntVector4::UnsignedIntVector4(const UnsignedIntVector4& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
	, w(copy.w)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
UnsignedIntVector4::UnsignedIntVector4(unsigned int initialX, unsigned int initialY, unsigned int initialZ, unsigned int initialW)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
	, w(initialW)
{
}

//*******************************************************************
void UnsignedIntVector4::GetXYZW(unsigned int& out_x, unsigned int& out_y, unsigned int& out_z, unsigned int& out_w) const
{
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
const unsigned int* UnsignedIntVector4::GetAsUnsignedIntArray() const
{
	return &x;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
unsigned int* UnsignedIntVector4::GetAsUnsignedIntArray()
{
	return &x;
}

//*******************************************************************
unsigned int UnsignedIntVector4::CalcLength() const
{
	return (unsigned int)sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

//*******************************************************************
unsigned int UnsignedIntVector4::CalcLengthSquared() const
{
	return ((x*x) + (y*y) + (z*z) +(w*w));
}

//*******************************************************************
void UnsignedIntVector4::SetXYZW(unsigned int newX, unsigned int newY, unsigned int newZ, unsigned int newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

//*******************************************************************
unsigned int UnsignedIntVector4::Normalize3D()
{
	unsigned int length = (unsigned int)sqrt((x*x) + (y*y) + (z*z));
	if (length > 0)
	{
		unsigned int invLength = (1 / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w = 1;
	}
	return length;
}

//*******************************************************************
unsigned int UnsignedIntVector4::Normalize4D()
{
	unsigned int length = (unsigned int)sqrt((x*x) + (y*y) + (z*z) + (w*w));
	if (length > 0)
	{
		unsigned int invLength = (1 / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w *= invLength;
	}
	return length;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
unsigned int UnsignedIntVector4::SetLength(unsigned int newLength)
{
	unsigned int oldLength = CalcLength();
	Normalize4D();
	ScaleUniform(newLength);
	return oldLength;
}

//*******************************************************************
void UnsignedIntVector4::ScaleUniform(unsigned int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

//*******************************************************************
void UnsignedIntVector4::ScaleNonUniform(const UnsignedIntVector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

//*******************************************************************
void UnsignedIntVector4::InverseScaleNonUniform(const UnsignedIntVector4& perAxisDivisors)
{
	x *= (1 / perAxisDivisors.x);
	y *= (1 / perAxisDivisors.y);
	z *= (1 / perAxisDivisors.z);
	w *= (1 / perAxisDivisors.w);
}

//*******************************************************************
bool UnsignedIntVector4::operator==(const UnsignedIntVector4& vectorToEqual) const
{
	return ((x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z) && (w == vectorToEqual.w));
}

//*******************************************************************
bool UnsignedIntVector4::operator!=(const UnsignedIntVector4& vectorToNotEqual) const
{
	return !(*(this) == vectorToNotEqual);
}

//*******************************************************************
const UnsignedIntVector4 UnsignedIntVector4::operator+(const UnsignedIntVector4& vectorToAdd) const
{
	return UnsignedIntVector4(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z, w + vectorToAdd.w);
}

//*******************************************************************
const UnsignedIntVector4 UnsignedIntVector4::operator-(const UnsignedIntVector4& vectorToSubtract) const
{
	return UnsignedIntVector4(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w);
}

//*******************************************************************
const UnsignedIntVector4 UnsignedIntVector4::operator*(unsigned int scale) const
{
	return UnsignedIntVector4(scale * x, scale * y, scale * z, scale * w);
}

//*******************************************************************
const UnsignedIntVector4 UnsignedIntVector4::operator*(const UnsignedIntVector4& perAxisScaleFactors) const
{
	return UnsignedIntVector4(perAxisScaleFactors.x * x, perAxisScaleFactors.y * y, perAxisScaleFactors.z * z, perAxisScaleFactors.w * w);
}

//*******************************************************************
const UnsignedIntVector4 UnsignedIntVector4::operator/(unsigned int inverseScale) const
{
	return UnsignedIntVector4(x / inverseScale, y / inverseScale, z / inverseScale, w / inverseScale);
}

//*******************************************************************
const UnsignedIntVector4 UnsignedIntVector4::operator/(const UnsignedIntVector4& perAxisInverseScaleFactors) const
{
	return UnsignedIntVector4(x / perAxisInverseScaleFactors.x, y / perAxisInverseScaleFactors.y, z / perAxisInverseScaleFactors.z, w / perAxisInverseScaleFactors.w);
}

//*******************************************************************
void UnsignedIntVector4::operator*=(unsigned int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

//*******************************************************************
void UnsignedIntVector4::operator*=(const UnsignedIntVector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

//*******************************************************************
void UnsignedIntVector4::operator+=(const UnsignedIntVector4& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
	w += vectorToAdd.w;
}

//*******************************************************************
void UnsignedIntVector4::operator-=(const UnsignedIntVector4& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}

//*******************************************************************
unsigned int CalcDistance(const UnsignedIntVector4& positionA, const UnsignedIntVector4& positionB)
{
	unsigned int distanceX = positionB.x - positionA.x;
	unsigned int distanceY = positionB.y - positionA.y;
	unsigned int distanceZ = positionB.z - positionA.z;
	unsigned int distanceW = positionB.w - positionA.w;
	unsigned int distance = (unsigned int)sqrt((distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ) + (distanceW*distanceW));
	return distance;
}

//*******************************************************************
unsigned int CalcDistanceSquared(const UnsignedIntVector4& positionA, const UnsignedIntVector4& positionB)
{
	unsigned int distanceX = positionB.x - positionA.x;
	unsigned int distanceY = positionB.y - positionA.y;
	unsigned int distanceZ = positionB.z - positionA.z;
	unsigned int distanceW = positionB.w - positionA.w;
	unsigned int distanceSquared = (distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ) + (distanceW*distanceW);
	return distanceSquared;
}

//*******************************************************************
const UnsignedIntVector4 operator*(unsigned int scale, const UnsignedIntVector4& vectorToScale)
{
	return UnsignedIntVector4(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z, scale * vectorToScale.z);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
unsigned int DotProduct(const UnsignedIntVector4& a, const UnsignedIntVector4& b)
{
	return ((a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w));
}

//*******************************************************************
const UnsignedIntVector4 Interpolate(const UnsignedIntVector4& start, const UnsignedIntVector4& end, unsigned int fractionToEnd)
{
	unsigned int fractionOfStart = 1 - fractionToEnd;
	UnsignedIntVector4 blended;
	blended.x = (fractionOfStart * start.x) + (fractionToEnd * end.x);
	blended.y = (fractionOfStart * start.y) + (fractionToEnd * end.y);
	blended.z = (fractionOfStart * start.z) + (fractionToEnd * end.z);
	blended.w = (fractionOfStart * start.w) + (fractionToEnd * end.w);
	return blended;
}