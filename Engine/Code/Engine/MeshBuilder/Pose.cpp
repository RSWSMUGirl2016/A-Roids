#include "Engine/MeshBuilder/Pose.hpp"
#include "Engine/MeshBuilder/Skeleton.hpp"


Matrix4 Pose::GetJointCurrentPoseGlobalTransform(unsigned int jointIndex, Skeleton* skeleton) const
{
    Matrix4 currentJointTransform = m_localTranforms[jointIndex];
    Matrix4 globalJointTransform = currentJointTransform;
    int currentJointParentIndex = skeleton->GetParentIndex(jointIndex);

    for (int localTransformsIndex = currentJointParentIndex; localTransformsIndex != -1; localTransformsIndex = skeleton->GetParentIndex(localTransformsIndex))
    {
        Matrix4 currentJointParent = m_localTranforms[currentJointParentIndex];
        globalJointTransform = MultiplyMatrix4ByMatrix4(currentJointTransform, currentJointParent);
        currentJointTransform = currentJointParent;
        int currentJointParentIndex2 = skeleton->GetParentIndex(jointIndex);
        currentJointParent = m_localTranforms[currentJointParentIndex2];
    }
    return globalJointTransform;
}


