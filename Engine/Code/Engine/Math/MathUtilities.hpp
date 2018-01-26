#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Matrix4.hpp"

const float fPI = 3.1415926535897932384626433832795f;
const float fSQRT_3_OVER_3 = 0.5773502691896257645091f;

int GetRandomIntLessThan(int maxValueNotInclusive);
int GetRandomIntInRange(int minValueInclusive, int maxValueInclusive);
float GetRandomFloatZeroToOne();
float GetRandomFloatInRange(float minInclusive, float maxInclusive);
Vector2 GetRandomVector2ZeroToOne();

float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd);
template<typename T> const T& ClampWithin(const T& value, const T& min, const T& max);

float ConvertDegreesToRadians(float degrees);
float ConvertRadiansToDegrees(float radians);
float CosDegrees(float degrees);
float SinDegrees(float degrees);
float TanDegrees(float degrees);
float Atan2Degrees(float y, float x);
float AcosDegrees(float x);

float SmoothStep(float inputZeroToOne);
float SmoothStep5(float inputZeroToOne);
float FastFloor(float number);


template<typename T> const T& ClampWithin(const T& value, const T& min, const T& max)
{
	if (value < min)
		return min;

	if (value > max)
		return max;

	return value;
}


