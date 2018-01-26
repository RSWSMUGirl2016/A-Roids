#include "Engine/Math/Math2D.hpp"

bool DoAABB2DsOverlap(const AABB2D& first, const AABB2D& second)
{
	if (first.m_mins.y > second.m_maxs.y || second.m_mins.y > first.m_maxs.y || first.m_mins.x > second.m_maxs.x || second.m_mins.x > first.m_maxs.x)
		return false;
	else
		return true;
}

bool DoDisc2DsOverlap(const Disc2D& first, const Disc2D& second)
{
	float distanceBetweenCenters = CalcDistance(first.center, second.center);
	float summationOfRadii = first.radius + second.radius;
	if (summationOfRadii > distanceBetweenCenters)
		return true;
	return false;
}

void BounceDiscs(Disc2D& first, Disc2D& second, Vector2& firstVelocity, Vector2& secondVelocity)
{
	if (!DoDisc2DsOverlap(first, second))
		return;

	Vector2 displacementFromFirstToSecond = second.center - first.center;
	Vector2 directionFromFirstToSecond = displacementFromFirstToSecond;
	directionFromFirstToSecond.Normalize();
	float distance = displacementFromFirstToSecond.CalcLength();
	float overlapDistance = (first.radius + second.radius) - distance;
	Vector2 firstPositionCorrection = directionFromFirstToSecond * (-0.5f * overlapDistance);
	second.center -= firstPositionCorrection;
	first.center += firstPositionCorrection;

	float speedTowardSecond = DotProduct(firstVelocity, directionFromFirstToSecond);
	if (speedTowardSecond > 0.f)
	{
		Vector2 velocityTowardSecond = speedTowardSecond * directionFromFirstToSecond;
		Vector2 velocityNotTowardsSecond = firstVelocity - directionFromFirstToSecond;
		firstVelocity = (-1.f * velocityTowardSecond) + velocityNotTowardsSecond;
		secondVelocity = (velocityTowardSecond) + velocityNotTowardsSecond;
	}
}

//void BounceAABB2DAndDisc2D(AABB2D& aabb2d, Disc2D& disc2d)
//{
//	float minimumPoisitonXForDisc2D = disc2d.center.x - disc2d.radius;
//	float minimumPoisitonYForDisc2D = disc2d.center.y - disc2d.radius;
//
//	float maximumPoisitonXForDisc2D = disc2d.center.x + disc2d.radius;
//	float maximumPoisitonYForDisc2D = disc2d.center.y + disc2d.radius;
//
//	if (aabb2d.IsPointInside(Vector2(maximumPoisitonXForDisc2D, disc2d.center.y)))
//	{
//		float overlapDistance = maximumPoisitonXForDisc2D - aabb2d.m_mins.x;
//		if (overlapDistance > 0.f)
//			disc2d.center.x -= overlapDistance;
//	}
//
//	if (aabb2d.IsPointInside(Vector2(minimumPoisitonYForDisc2D, disc2d.center.y)))
//	{
//		float overlapDistance = maximumPoisitonXForDisc2D - aabb2d.m_mins.x;
//		if (overlapDistance > 0.f)
//			disc2d.center.x -= overlapDistance;
//	}
//}
