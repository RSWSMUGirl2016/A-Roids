#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/Vector3.hpp"
#include <iostream>

using namespace std;

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector3::Vector3()
	: x(0.f)
	, y(0.f)
	, z(0.f)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector3::Vector3(const Vector3& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector3::Vector3(float initialX, float initialY, float initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}

//*******************************************************************
void Vector3::GetXYZ(float& out_x, float& out_y, float& out_z) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
const float* Vector3::GetAsFloatArray() const
{
	return &x;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float* Vector3::GetAsFloatArray()
{
	return &x;
}

//*******************************************************************
float Vector3::CalcLength() const
{
	return sqrt((x*x) + (y*y) + (z*z));
}

//*******************************************************************
float Vector3::CalcLengthSquared() const
{
	return ((x*x) + (y*y) + (z*z));
}

//*******************************************************************
void Vector3::SetXYZ(float newX, float newY, float newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float Vector3::Normalize()
{
	float length = sqrt((x*x) + (y*y) + (z*z));
	if (length > 0.f)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
	return length;
}

//*******************************************************************
float Vector3::GetLength() const
{
    return sqrtf(DotProduct(*this, *this));
}

//*******************************************************************
Vector3 Vector3::GetNormalized() const
{
    float const il = 1.0f / GetLength();
    return Vector3(x * il, y * il, z * il);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float Vector3::SetLength(float newLength)
{
	float oldLength = CalcLength();
	Normalize();
	ScaleUniform(newLength);
	return oldLength;
}

//*******************************************************************
void Vector3::ScaleUniform(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

//*******************************************************************
void Vector3::ScaleNonUniform(const Vector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

//*******************************************************************
void Vector3::InverseScaleNonUniform(const Vector3& perAxisDivisors)
{
	x *= (1 / perAxisDivisors.x);
	y *= (1 / perAxisDivisors.y);
	z *= (1 / perAxisDivisors.z);
}

//*******************************************************************
float Vector3::CalcHeadingYawDegrees() const
{
	return ConvertRadiansToDegrees(atan2(y, x));
}

//*******************************************************************
bool Vector3::operator==(const Vector3& vectorToEqual) const
{
	return ((x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z));
}

//*******************************************************************
bool Vector3::operator!=(const Vector3& vectorToNotEqual) const
{
	return !(*(this) == vectorToNotEqual);
}

//*******************************************************************
const Vector3 Vector3::operator+(const Vector3& vectorToAdd) const
{
	return Vector3(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z);
}

//*******************************************************************
const Vector3 Vector3::operator-(const Vector3& vectorToSubtract) const
{
	return Vector3(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z);
}

//*******************************************************************
const Vector3 Vector3::operator*(float scale) const
{
	return Vector3(scale * x, scale * y, scale * z);
}

//*******************************************************************
const Vector3 Vector3::operator*(const Vector3& perAxisScaleFactors) const
{
	return Vector3(perAxisScaleFactors.x * x, perAxisScaleFactors.y * y, perAxisScaleFactors.z * z);
}

//*******************************************************************
const Vector3 Vector3::CrossProduct(const Vector3& other) const
{
    return Vector3(
        y * other.z     - z * other.y, 
        -x * other.z    + z * other.x, 
        x * other.y     - y * other.x
    );
}

//*******************************************************************
const Vector3 Vector3::operator/(float inverseScale) const
{
	return Vector3(x / inverseScale, y / inverseScale, z / inverseScale);
}

//*******************************************************************
const Vector3 Vector3::operator/(const Vector3& perAxisInverseScaleFactors) const
{
	return Vector3(x / perAxisInverseScaleFactors.x, y / perAxisInverseScaleFactors.y, z / perAxisInverseScaleFactors.z);
}

//*******************************************************************
void Vector3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

//*******************************************************************
void Vector3::operator*=(const Vector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

//*******************************************************************
void Vector3::operator+=(const Vector3& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

//*******************************************************************
void Vector3::operator-=(const Vector3& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

//*******************************************************************
float CalcDistance(const Vector3& positionA, const Vector3& positionB)
{
	float distanceX = positionB.x - positionA.x;
	float distanceY = positionB.y - positionA.y;
	float distanceZ = positionB.z - positionA.z;
	float distance = (float)sqrt((distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ));
	return distance;
}

//*******************************************************************
float CalcDistanceSquared(const Vector3& positionA, const Vector3& positionB)
{
	float distanceX = positionB.x - positionA.x;
	float distanceY = positionB.y - positionA.y;
	float distanceZ = positionB.z - positionA.z;
	float distanceSquared = (distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ);
	return distanceSquared;
}

//*******************************************************************
const Vector3 operator*(float scale, const Vector3& vectorToScale)
{
	return Vector3(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float DotProduct(const Vector3& a, const Vector3& b)
{
	return ((a.x*b.x) + (a.y*b.y) + (a.z*b.z));
}

//*******************************************************************
const Vector3 Interpolate(const Vector3& start, const Vector3& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	Vector3 blended;
	blended.x = (fractionOfStart * start.x) + (fractionToEnd * end.x);
	blended.y = (fractionOfStart * start.y) + (fractionToEnd * end.y);
	blended.z = (fractionOfStart * start.z) + (fractionToEnd * end.z);
	return blended;
}