#pragma once
#include "Engine/Math/Matrix4.hpp"
#include <string>
#include <vector>

class Skeleton;

class Pose
{
public:
    std::vector<Matrix4> m_localTranforms;

    Matrix4 GetJointCurrentPoseGlobalTransform(unsigned int jointIndex, Skeleton* skeleton) const;
};