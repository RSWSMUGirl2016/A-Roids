#include "Engine/Math/MathUtilities.hpp"

#include <math.h>
#include <stdlib.h>

//*******************************************************************
int GetRandomIntLessThan(int maxValueNotInclusive)
{
	return rand() % maxValueNotInclusive;
}

//*******************************************************************
int GetRandomIntInRange(int minValueInclusive, int maxValueInclusive)
{
	int differenceBetweenMinAndMaxPlusOne = (maxValueInclusive - minValueInclusive) + 1;
	int randomIntRange = rand() % differenceBetweenMinAndMaxPlusOne;
	return minValueInclusive + randomIntRange;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float GetRandomFloatZeroToOne()
{
	int randomNumber = rand();
	return (float)randomNumber / (float)RAND_MAX;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float GetRandomFloatInRange(float minInclusive, float maxInclusive)
{
	float randomZeroToOne = GetRandomFloatZeroToOne();
	return minInclusive + ((maxInclusive - minInclusive) * randomZeroToOne);
}

//*******************************************************************
Vector2 GetRandomVector2ZeroToOne()
{
	float randomFloatZeroToOne = GetRandomFloatZeroToOne();
	return Vector2(randomFloatZeroToOne, randomFloatZeroToOne);
}


//*******************************************************************
float RangeMapFloat(float inValue, float inStart, float inEnd, float outStart, float outEnd)
{
	if (inEnd == inStart)
		return 0.5f * (outStart + outEnd);

	float fractionInInRange = (inValue - inStart) / (inEnd - inStart);
	float outValue = outStart + (fractionInInRange * (outEnd - outStart));
	return outValue;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float ConvertDegreesToRadians(float degrees)
{
	return degrees * (fPI / 180.f);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float ConvertRadiansToDegrees(float radians)
{
	return (180 * radians) / fPI;
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float CosDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return (float)cos(radians);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
float SinDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return (float)sin(radians);
}

//*******************************************************************
float TanDegrees(float degrees)
{
	float radians = ConvertDegreesToRadians(degrees);
	return (float)tan(radians);
}

//*******************************************************************
float Atan2Degrees(float y, float x)
{
	float radians = (float) atan2(y, x);
	return (float)ConvertRadiansToDegrees(radians);
}

//*******************************************************************
float AcosDegrees(float x)
{
	float radians = (float)acos(x);
	return (float)ConvertRadiansToDegrees(radians);
}

//*******************************************************************
float SmoothStep(float inputZeroToOne)
{
	return (3.f * inputZeroToOne * inputZeroToOne) - (2.f * inputZeroToOne * inputZeroToOne * inputZeroToOne);
}

float SmoothStep5(float inputZeroToOne)
{
	return (6.f * inputZeroToOne * inputZeroToOne * inputZeroToOne * inputZeroToOne * inputZeroToOne) - (15.f * inputZeroToOne * inputZeroToOne * inputZeroToOne * inputZeroToOne) + (10.f * inputZeroToOne * inputZeroToOne * inputZeroToOne);
}

//*******************************************************************
float FastFloor(float number)
{
	return (float)floor(number);
}

