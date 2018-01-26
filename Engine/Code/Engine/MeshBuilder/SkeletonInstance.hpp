#pragma once
#include "Engine/MeshBuilder/Motion.hpp"
#include "Engine/MeshBuilder/Pose.hpp"
#include "Engine/MeshBuilder/Skeleton.hpp"
#include "Engine/MeshBuilder/StructuredBuffer.hpp"

class SkeletonInstance
{
public:
    Skeleton *m_skeleton; // skeleton we're applying poses to.  Used for heirachy information.
    Pose m_currentPose; // my current skeletons pose.
    StructuredBuffer* m_skinTransforms;

    SkeletonInstance();
    ~SkeletonInstance();
    SkeletonInstance(Skeleton * skeleton);

    // Excercise:  How do we evaluate a global transform for this joint if the pose only contains 
    // local transforms?
    // Hint:  You will need "get_joint_parent" below.
    Matrix4 GetJointCurrentPoseGlobalTransform(unsigned int jointIndex) const;
    Matrix4 GetJointBindPoseGlobalTransform(unsigned int jointIndex) const;
    // returns INVALID_INDEX (-1) if no parent exists.
    // Use the skeleton to get this information.
    int GetJointParent(unsigned int jointIndex) const;

    void ApplyMotion(Motion const *motion, float const time)
    {
        motion->Evaluate(m_currentPose, time);
    }
};