#include "Engine/Math/MathUtilities.hpp"
#include "Engine/Math/Vector2.hpp"
#include <iostream>

using namespace std;

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector2::Vector2()
	: x(0.f)
	, y(0.f)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector2::Vector2(const Vector2& copy)
	: x(copy.x)
	, y(copy.y)
{
}

//*******************************************************************
// Credit to Squirrel Eiserloh
Vector2::Vector2(float initialX, float initialY)
	: x(initialX)
	, y(initialY)
{
}

//*******************************************************************
void Vector2::GetXY(float& out_x, float& out_y) const
{
	out_x = x;
	out_y = y;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
const float* Vector2::GetAsFloatArray() const
{
	return &x;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float* Vector2::GetAsFloatArray()
{
	return &x;
}

//*******************************************************************
float Vector2::CalcLength() const
{
	return sqrt((x*x) + (y*y));
}

//*******************************************************************
float Vector2::CalcLengthSquared() const
{
	return ((x*x) + (y*y));
}

//*******************************************************************
float Vector2::CalcHeadingDegrees() const
{
	return ConvertRadiansToDegrees(atan2( y, x) );
}

//*******************************************************************
float Vector2::CalcHeadingRadians() const
{
	return atan2( y, x );
}

//*******************************************************************
void Vector2::SetXY(float newX, float newY)
{
	x = newX;
	y = newY;
}

//*******************************************************************
void Vector2::Rotate90Degrees()
{
	y = x;
	x = (-1.f) * y;
}

//*******************************************************************
void Vector2::RotateNegative90Degrees()
{
	x = y;
	y = (-1.f) * x;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
void Vector2::RotateDegrees(float degrees)
{
	float R = sqrt((x*x) + (y*y));
	float myDegrees = Atan2Degrees(y, x);
	myDegrees += degrees;
	x = R * CosDegrees(myDegrees);
	y = R * SinDegrees(myDegrees);
}

//*******************************************************************
void Vector2::RotateRadians(float radians)
{
	float R = sqrt((x*x) + (y*y));
	float myRadians = atan2(y, x);
	myRadians += radians;
	x = R;
	y = myRadians;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float Vector2::Normalize()
{
	float length = sqrt((x*x) + (y*y));
	if (length > 0.f)
	{
		float invLength = (1.0f / length);
		x *= invLength;
		y *= invLength;
	}
	return length;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float Vector2::SetLength(float newLength)
{
	float oldLength = CalcLength();
	Normalize();
	ScaleUniform(newLength);
	return oldLength;
}

//*******************************************************************
void Vector2::ScaleUniform(float scale)
{
	x *= scale;
	y *= scale;
}

//*******************************************************************
void Vector2::ScaleNonUniform(const Vector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

//*******************************************************************
void Vector2::InverseScaleNonUniform(const Vector2& perAxisDivisors)
{
	x *= (1 / perAxisDivisors.x);
	y *= (1 / perAxisDivisors.y);
}

//*******************************************************************
void Vector2::SetUnitLengthAndYawDegrees(float yawDegrees)
{
	SetLength(1.f);
	x = CosDegrees(yawDegrees);
	y = SinDegrees(yawDegrees);
}

//*******************************************************************
void Vector2::SetUnitLengthAndYawRadians(float yawRadians)
{
	SetLength(1.f);
	x = cos(yawRadians);
	y = sin(yawRadians);
}

void Vector2::SetLengthAndYawDegrees(float newLength, float yawDegrees)
{
	SetLength(newLength);
	x = newLength*CosDegrees(yawDegrees);
	y = newLength*SinDegrees(yawDegrees);
}

void Vector2::SetLengthAndYawRadians(float newLength, float yawRadians)
{
	SetLength(newLength);
	x = newLength*cos(yawRadians);
	y = newLength*sin(yawRadians);
}

//*******************************************************************
bool Vector2::operator==(const Vector2& vectorToEqual) const
{
	return ((x == vectorToEqual.x) && (y == vectorToEqual.y));
}

//*******************************************************************
bool Vector2::operator!=(const Vector2& vectorToNotEqual) const
{
	return !(*(this) == vectorToNotEqual);
}

//*******************************************************************
const Vector2 Vector2::operator+(const Vector2& vectorToAdd) const
{
	return Vector2(x + vectorToAdd.x, y + vectorToAdd.y);
}

//*******************************************************************
const Vector2 Vector2::operator-(const Vector2& vectorToSubtract) const
{
	return Vector2(x - vectorToSubtract.x, y - vectorToSubtract.y);
}

//*******************************************************************
const Vector2 Vector2::operator*(float scale) const
{
	return Vector2(scale * x, scale * y);
}

//*******************************************************************
const Vector2 Vector2::operator*(const Vector2& perAxisScaleFactors) const
{
	return Vector2(perAxisScaleFactors.x * x, perAxisScaleFactors.y * y);
}

//*******************************************************************
const Vector2 Vector2::operator/(float inverseScale) const
{
	return Vector2(x / inverseScale, y / inverseScale);
}

//*******************************************************************
const Vector2 Vector2::operator/(const Vector2& perAxisInverseScaleFactors) const
{
	return Vector2(x / perAxisInverseScaleFactors.x, y / perAxisInverseScaleFactors.y);
}

//*******************************************************************
void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

//*******************************************************************
void Vector2::operator*=(const Vector2& perAxisScaleFactors)
{
	x *= perAxisScaleFactors.x;
	y *= perAxisScaleFactors.y;
}

//*******************************************************************
void Vector2::operator+=(const Vector2& vectorToAdd)
{
	x += vectorToAdd.x;
	y += vectorToAdd.y;
}

//*******************************************************************
void Vector2::operator-=(const Vector2& vectorToSubtract)
{
	x -= vectorToSubtract.x;
	y -= vectorToSubtract.y;
}

//*******************************************************************
float CalcDistance(const Vector2& positionA, const Vector2& positionB)
{
 	float distanceX = positionB.x - positionA.x;
	float distanceY = positionB.y - positionA.y;
	float distance = (float)sqrt((distanceX*distanceX) + (distanceY*distanceY));
	return distance;
}

//*******************************************************************
float CalcDistanceSquared(const Vector2& positionA, const Vector2& positionB)
{
	float distanceX = positionB.x - positionA.x;
	float distanceY = positionB.y - positionA.y;
	float distanceSquared = (distanceX*distanceX) + (distanceY*distanceY);
	return distanceSquared;
}

//*******************************************************************
const Vector2 operator*(float scale, const Vector2& vectorToScale)
{
	return Vector2(scale * vectorToScale.x, scale * vectorToScale.y);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float DotProduct(const Vector2& a, const Vector2& b)
{
	return ((a.x*b.x) + (a.y*b.y));
}

//*******************************************************************
const Vector2 Interpolate(const Vector2& start, const Vector2& end, float fractionToEnd)
{
	float fractionOfStart = 1.f - fractionToEnd;
	Vector2 blended;
	blended.x = (fractionOfStart * start.x) + (fractionToEnd * end.x);
	blended.y = (fractionOfStart * start.y) + (fractionToEnd * end.y);
	return blended;
}