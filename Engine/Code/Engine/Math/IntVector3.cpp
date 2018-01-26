#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/IntVector3.hpp"
#include <iostream>

using namespace std;

//*******************************************************************
// Credit to Squirrel Eiserloh
IntVector3::IntVector3()
	: x(0)
	, y(0)
	, z(0)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
IntVector3::IntVector3(const IntVector3& copy)
	: x(copy.x)
	, y(copy.y)
	, z(copy.z)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
IntVector3::IntVector3(int initialX, int initialY, int initialZ)
	: x(initialX)
	, y(initialY)
	, z(initialZ)
{
}

//*******************************************************************
void IntVector3::GetXYZ(int& out_x, int& out_y, int& out_z) const
{
	out_x = x;
	out_y = y;
	out_z = z;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
const int* IntVector3::GetAsFloatArray() const
{
	return &x;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
int* IntVector3::GetAsFloatArray()
{
	return &x;
}

//*******************************************************************
int IntVector3::CalcLength() const
{
	return (int) sqrt((x*x) + (y*y) + (z*z));
}

//*******************************************************************
int IntVector3::CalcLengthSquared() const
{
	return ((x*x) + (y*y) + (z*z));
}

//*******************************************************************
void IntVector3::SetXYZ(int newX, int newY, int newZ)
{
	x = newX;
	y = newY;
	z = newZ;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
int IntVector3::Normalize()
{
	int length = (int) sqrt((x*x) + (y*y) + (z*z));
	if (length > 0)
	{
		int invLength = (int)(1 / length);
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
	return length;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
int IntVector3::SetLength(int newLength)
{
	int oldLength = CalcLength();
	Normalize();
	ScaleUniform(newLength);
	return oldLength;
}

//*******************************************************************
void IntVector3::ScaleUniform(int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

//*******************************************************************
void IntVector3::ScaleNonUniform(const IntVector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

//*******************************************************************
void IntVector3::InverseScaleNonUniform(const IntVector3& perAxisDivisors)
{
	x *= (1 / perAxisDivisors.x);
	y *= (1 / perAxisDivisors.y);
	z *= (1 / perAxisDivisors.z);
}

//*******************************************************************
bool IntVector3::operator==(const IntVector3& vectorToEqual) const
{
	return ((x == vectorToEqual.x) && (y == vectorToEqual.y) && (z == vectorToEqual.z));
}

//*******************************************************************
bool IntVector3::operator!=(const IntVector3& vectorToNotEqual) const
{
	return !(*(this) == vectorToNotEqual);
}

//*******************************************************************
const IntVector3 IntVector3::operator+(const IntVector3& vectorToAdd) const
{
	return IntVector3(x + vectorToAdd.x, y + vectorToAdd.y, z + vectorToAdd.z);
}

//*******************************************************************
const IntVector3 IntVector3::operator-(const IntVector3& vectorToSubtract) const
{
	return IntVector3(x - vectorToSubtract.x, y - vectorToSubtract.y, z - vectorToSubtract.z);
}

//*******************************************************************
const IntVector3 IntVector3::operator*(int scale) const
{
	return IntVector3(scale * x, scale * y, scale * z);
}

//*******************************************************************
const IntVector3 IntVector3::operator*(const IntVector3& perAxisScaleFactors) const
{
	return IntVector3(perAxisScaleFactors.x * x, perAxisScaleFactors.y * y, perAxisScaleFactors.z * z);
}

//*******************************************************************
const IntVector3 IntVector3::operator/(int inverseScale) const
{
	return IntVector3(x / inverseScale, y / inverseScale, z / inverseScale);
}

//*******************************************************************
const IntVector3 IntVector3::operator/(const IntVector3& perAxisInverseScaleFactors) const
{
	return IntVector3(x / perAxisInverseScaleFactors.x, y / perAxisInverseScaleFactors.y, z / perAxisInverseScaleFactors.z);
}

//*******************************************************************
void IntVector3::operator*=(int scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
}

//*******************************************************************
void IntVector3::operator*=(const IntVector3& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
	z *= perAxisScaleFactors.z;
}

//*******************************************************************
void IntVector3::operator+=(const IntVector3& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
	z += vectorToAdd.z;
}

//*******************************************************************
void IntVector3::operator-=(const IntVector3& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
	z -= vectorToSubtract.z;
}

//*******************************************************************
int CalcDistance(const IntVector3& positionA, const IntVector3& positionB)
{
	int distanceX = positionB.x - positionA.x;
	int distanceY = positionB.y - positionA.y;
	int distanceZ = positionB.z - positionA.z;
	int distance = (int)sqrt((distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ));
	return distance;
}

//*******************************************************************
int CalcDistanceSquared(const IntVector3& positionA, const IntVector3& positionB)
{
	int distanceX = positionB.x - positionA.x;
	int distanceY = positionB.y - positionA.y;
	int distanceZ = positionB.z - positionA.z;
	int distanceSquared = (distanceX*distanceX) + (distanceY*distanceY) + (distanceZ*distanceZ);
	return distanceSquared;
}

//*******************************************************************
const IntVector3 operator*(int scale, const IntVector3& vectorToScale)
{
	return IntVector3(scale * vectorToScale.x, scale * vectorToScale.y, scale * vectorToScale.z);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
int DotProduct(const IntVector3& a, const IntVector3& b)
{
	return ((a.x*b.x) + (a.y*b.y) + (a.z*b.z));
}

//*******************************************************************
const IntVector3 Interpolate(const IntVector3& start, const IntVector3& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	IntVector3 blended;
	blended.x = (int)((fractionOfStart * start.x) + (fractionToEnd * end.x));
	blended.y = (int)((fractionOfStart * start.y) + (fractionToEnd * end.y));
	blended.z = (int)((fractionOfStart * start.z) + (fractionToEnd * end.z));
	return blended;
}