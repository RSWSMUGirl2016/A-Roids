#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/Vector4.hpp"
#include <iostream>

using namespace std;

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector4::Vector4()
	: x(0.f)
	, y(0.f)
	, z(0.f)
	, w(0.f)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector4::Vector4(const Vector4& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
	, w(copy.w)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector4::Vector4(float initialX, float initialY, float initialZ, float initialW)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
	, w(initialW)
{
}

//*******************************************************************
void Vector4::GetXYZW(float& out_x, float& out_y, float& out_z, float& out_w) const
{
	out_x = x;
	out_y = y;
	out_z = z;
	out_w = w;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
const float* Vector4::GetAsFloatArray() const
{
	return &x;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float* Vector4::GetAsFloatArray()
{
	return &x;
}

//*******************************************************************
float Vector4::CalcLength() const
{
	return sqrt((x*x) + (y*y) + (z*z) + (w*w));
}

//*******************************************************************
float Vector4::CalcLengthSquared() const
{
	return ((x*x) + (y*y) + (z*z) +(w*w));
}

//*******************************************************************
void Vector4::SetXYZW(float newX, float newY, float newZ, float newW)
{
	x = newX;
	y = newY;
	z = newZ;
	w = newW;
}

//*******************************************************************
float Vector4::Normalize3D()
{
	float length = sqrt((x*x) + (y*y) + (z*z));
	if (length > 0.f)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w = 1.f;
	}
	return length;
}

//*******************************************************************
float Vector4::Normalize4D()
{
	float length = sqrt((x*x) + (y*y) + (z*z) + (w*w));
	if (length > 0.f)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		w *= invLength;
	}
	return length;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float Vector4::SetLength(float newLength)
{
	float oldLength = CalcLength();
	Normalize4D();
	ScaleUniform(newLength);
	return oldLength;
}

//*******************************************************************
void Vector4::ScaleUniform(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

//*******************************************************************
void Vector4::ScaleNonUniform(const Vector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

//*******************************************************************
void Vector4::InverseScaleNonUniform(const Vector4& perAxisDivisors)
{
	x *= (1 / perAxisDivisors.x);
	y *= (1 / perAxisDivisors.y);
	z *= (1 / perAxisDivisors.z);
	w *= (1 / perAxisDivisors.w);
}

//*******************************************************************
bool Vector4::operator==(const Vector4& vectorToEqual) const
{
	return ((x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z) && (w == vectorToEqual.w));
}

//*******************************************************************
bool Vector4::operator!=(const Vector4& vectorToNotEqual) const
{
	return !(*(this) == vectorToNotEqual);
}

//*******************************************************************
const Vector4 Vector4::operator+(const Vector4& vectorToAdd) const
{
	return Vector4(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z, w + vectorToAdd.w);
}

//*******************************************************************
const Vector4 Vector4::operator-(const Vector4& vectorToSubtract) const
{
	return Vector4(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z, w - vectorToSubtract.w);
}

//*******************************************************************
const Vector4 Vector4::operator*(float scale) const
{
	return Vector4(scale * x, scale * y, scale * z, scale * w);
}

//*******************************************************************
const Vector4 Vector4::operator*(const Vector4& perAxisScaleFactors) const
{
	return Vector4(perAxisScaleFactors.x * x, perAxisScaleFactors.y * y, perAxisScaleFactors.z * z, perAxisScaleFactors.w * w);
}

//*******************************************************************
const Vector4 Vector4::operator/(float inverseScale) const
{
	return Vector4(x / inverseScale, y / inverseScale, z / inverseScale, w / inverseScale);
}

//*******************************************************************
const Vector4 Vector4::operator/(const Vector4& perAxisInverseScaleFactors) const
{
	return Vector4(x / perAxisInverseScaleFactors.x, y / perAxisInverseScaleFactors.y, z / perAxisInverseScaleFactors.z, w / perAxisInverseScaleFactors.w);
}

//*******************************************************************
void Vector4::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	w *= scale;
}

//*******************************************************************
void Vector4::operator*=(const Vector4& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
	w *= perAxisScaleFactors.w;
}

//*******************************************************************
void Vector4::operator+=(const Vector4& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
	w += vectorToAdd.w;
}

//*******************************************************************
void Vector4::operator-=(const Vector4& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
	w -= vectorToSubtract.w;
}

//*******************************************************************
float CalcDistance(const Vector4& positionA, const Vector4& positionB)
{
	float distanceX = positionB.x - positionA.x;
	float distanceY = positionB.y - positionA.y;
	float distanceZ = positionB.z - positionA.z;
	float distanceW = positionB.w - positionA.w;
	float distance = (float)sqrt((distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ) + (distanceW*distanceW));
	return distance;
}

//*******************************************************************
float CalcDistanceSquared(const Vector4& positionA, const Vector4& positionB)
{
	float distanceX = positionB.x - positionA.x;
	float distanceY = positionB.y - positionA.y;
	float distanceZ = positionB.z - positionA.z;
	float distanceW = positionB.w - positionA.w;
	float distanceSquared = (distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ) + (distanceW*distanceW);
	return distanceSquared;
}

//*******************************************************************
const Vector4 operator*(float scale, const Vector4& vectorToScale)
{
	return Vector4(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z, scale * vectorToScale.z);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float DotProduct(const Vector4& a, const Vector4& b)
{
	return ((a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w));
}

//*******************************************************************
const Vector4 Interpolate(const Vector4& start, const Vector4& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	Vector4 blended;
	blended.x = (fractionOfStart * start.x) + (fractionToEnd * end.x);
	blended.y = (fractionOfStart * start.y) + (fractionToEnd * end.y);
	blended.z = (fractionOfStart * start.z) + (fractionToEnd * end.z);
	blended.w = (fractionOfStart * start.w) + (fractionToEnd * end.w);
	return blended;
}