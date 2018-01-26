#include "Engine/Math/Math3D.hpp"

bool DoAABB3DsOverlap(const AABB3D& first, const AABB3D& second)
{
	if (first.m_mins.y > second.m_maxs.y || second.m_mins.y > first.m_maxs.y || first.m_mins.x > second.m_maxs.x || second.m_mins.x > first.m_maxs.x)
		return false;
	else
		return true;
}

bool DoDisc3DsOverlap(const Sphere3D& first, const Sphere3D& second)
{
	float distanceBetweenCenters = CalcDistance(first.center, second.center);
	float summationOfRadii = first.radius + second.radius;
	if (summationOfRadii > distanceBetweenCenters)
		return true;
	return false;
}

