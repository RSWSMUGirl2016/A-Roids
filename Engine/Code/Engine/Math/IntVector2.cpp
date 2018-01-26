#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/IntVector2.hpp"
#include <iostream>

using namespace std;

//*******************************************************************
IntVector2::IntVector2()
	: x(0)
	, y(0)
{
}

//*******************************************************************
IntVector2::IntVector2(const IntVector2& copy)
	: x(copy.x)
	, y(copy.y)
{
}

//*******************************************************************
IntVector2::IntVector2(int initialX, int initialY)
	: x(initialX)
	, y(initialY)
{
}

//*******************************************************************
void IntVector2::GetXY(int& out_x, int& out_y) const
{
	out_x = x;
	out_y = y;
}

//*******************************************************************
const int* IntVector2::GetAsFloatArray() const
{
	return &x;
}

//*******************************************************************
int* IntVector2::GetAsFloatArray()
{
	return &x;
}

//*******************************************************************
int IntVector2::CalcLength() const
{
	return (int) sqrt((x*x) + (y*y));
}

//*******************************************************************
int IntVector2::CalcLengthSquared() const
{
	return ((x*x) + (y*y));
}

//*******************************************************************
float IntVector2::CalcHeadingDegrees() const
{
	return (float) ConvertRadiansToDegrees(atan2((float)y, (float)x));
}

//*******************************************************************
float IntVector2::CalcHeadingRadians() const
{
	return (float) atan2((float)y, (float)x);
}

//*******************************************************************
void IntVector2::SetXY(int newX, int newY)
{
	x = newX;
	y = newY;
}

//*******************************************************************
void IntVector2::Rotate90Degrees()
{
	y = x;
	x = (-1) * y;
}

//*******************************************************************
void IntVector2::RotateNegative90Degrees()
{
	x = y;
	y = (-1) * x;
}

//*******************************************************************
void IntVector2::RotateDegrees(float degrees)
{
	int R = (int) sqrt((x*x) + (y*y));
	float myDegrees = Atan2Degrees((float)y, (float)x);
	myDegrees += degrees;
	x = R * (int)CosDegrees(myDegrees);
	y = R * (int)SinDegrees(myDegrees);
}

//*******************************************************************
void IntVector2::RotateRadians(float radians)
{
	int R = (int)sqrt((x*x) + (y*y));
	float myRadians = atan2((float)y, (float)x);
	myRadians += radians;
	x = R;
	y = (int)myRadians;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
int IntVector2::Normalize()
{
	int length = (int)sqrt((x*x) + (y*y));
	if (length > 0.f)
	{
		int invLength = (1 / length);
		x *= invLength;
		y *= invLength;
	}
	return length;
}

//*******************************************************************
int IntVector2::SetLength(int newLength)
{
	int oldLength = CalcLength();
	Normalize();
	ScaleUniform(newLength);
	return oldLength;
}

//*******************************************************************
void IntVector2::ScaleUniform(int scale)
{
	x *= scale;
	y *= scale;
}

//*******************************************************************
void IntVector2::ScaleNonUniform(const IntVector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

//*******************************************************************
void IntVector2::InverseScaleNonUniform(const IntVector2& perAxisDivisors)
{
	x *= (1 / perAxisDivisors.x);
	y *= (1 / perAxisDivisors.y);
}

//*******************************************************************
void IntVector2::SetUnitLengthAndYawDegrees(float yawDegrees)
{
	SetLength(1);
	x = (int)CosDegrees(yawDegrees);
	y = (int)SinDegrees(yawDegrees);
}

//*******************************************************************
void IntVector2::SetUnitLengthAndYawRadians(float yawRadians)
{
	SetLength(1);
	x = (int)cos(yawRadians);
	y = (int)sin(yawRadians);
}

void IntVector2::SetLengthAndYawDegrees(int newLength, float yawDegrees)
{
	SetLength(newLength);
	x = newLength*((int)CosDegrees(yawDegrees));
	y = newLength*((int)SinDegrees(yawDegrees));
}

void IntVector2::SetLengthAndYawRadians(int newLength, float yawRadians)
{
	SetLength(newLength);
	x = newLength*((int)cos(yawRadians));
	y = newLength*((int)sin(yawRadians));
}

//*******************************************************************
bool IntVector2::operator==(const IntVector2& vectorToEqual) const
{
	return ((x == vectorToEqual.x) && (y == vectorToEqual.y));
}

//*******************************************************************
bool IntVector2::operator!=(const IntVector2& vectorToNotEqual) const
{
	return !(*(this) == vectorToNotEqual);
}

//*******************************************************************
const IntVector2 IntVector2::operator+(const IntVector2& vectorToAdd) const
{
	return IntVector2(x + vectorToAdd.x, y + vectorToAdd.y);
}

//*******************************************************************
const IntVector2 IntVector2::operator-(const IntVector2& vectorToSubtract) const
{
	return IntVector2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

//*******************************************************************
const IntVector2 IntVector2::operator*(int scale) const
{
	return IntVector2(scale * x, scale * y);
}

//*******************************************************************
const IntVector2 IntVector2::operator*(const IntVector2& perAxisScaleFactors) const
{
	return IntVector2(perAxisScaleFactors.x * x, perAxisScaleFactors.y * y);
}

//*******************************************************************
const IntVector2 IntVector2::operator/(int inverseScale) const
{
	return IntVector2(x / inverseScale, y / inverseScale);
}

//*******************************************************************
const IntVector2 IntVector2::operator/(const IntVector2& perAxisInverseScaleFactors) const
{
	return IntVector2(x / perAxisInverseScaleFactors.x, y / perAxisInverseScaleFactors.y);
}

//*******************************************************************
void IntVector2::operator*=(int scale)
{
	x *= scale;
	y *= scale;
}

//*******************************************************************
void IntVector2::operator*=(const IntVector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

//*******************************************************************
void IntVector2::operator+=(const IntVector2& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

//*******************************************************************
void IntVector2::operator-=(const IntVector2& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

//*******************************************************************
int CalcDistance(const IntVector2& positionA, const IntVector2& positionB)
{
	int distanceX = positionB.x - positionA.x;
	int distanceY = positionB.y - positionA.y;
	int distance = (int)sqrt((distanceX*distanceX) + (distanceY*distanceY));
	return distance;
}

//*******************************************************************
int CalcDistanceSquared(const IntVector2& positionA, const IntVector2& positionB)
{
	int distanceX = positionB.x - positionA.x;
	int distanceY = positionB.y - positionA.y;
	int distanceSquared = (distanceX*distanceX) + (distanceY*distanceY);
	return distanceSquared;
}

//*******************************************************************
const IntVector2 operator*(int scale, const IntVector2& vectorToScale)
{
	return IntVector2(scale * vectorToScale.x, scale * vectorToScale.y);
}

//*******************************************************************
int DotProduct(const IntVector2& a, const IntVector2& b)
{
	return ((a.x*b.x) + (a.y*b.y));
}

//*******************************************************************
const IntVector2 Interpolate(const IntVector2& start, const IntVector2& end, float fractionToEnd)
{
	int fractionOfStart = 1 - (int)fractionToEnd;
	IntVector2 blended;
	blended.x = (fractionOfStart * start.x) + ((int)fractionToEnd * end.x);
	blended.y = (fractionOfStart * start.y) + ((int)fractionToEnd * end.y);
	return blended;
}