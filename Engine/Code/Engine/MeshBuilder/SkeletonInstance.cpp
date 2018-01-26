#include "Engine/MeshBuilder/SkeletonInstance.hpp"

//*******************************************************************
SkeletonInstance::SkeletonInstance()
{
}

//*******************************************************************
SkeletonInstance::SkeletonInstance(Skeleton * skeleton)
    : m_skeleton(skeleton)
{

}

//*******************************************************************
SkeletonInstance::~SkeletonInstance()
{
}

//*******************************************************************
Matrix4 SkeletonInstance::GetJointCurrentPoseGlobalTransform(unsigned int jointIndex) const
{
    Matrix4 currentJointTransform = m_currentPose.m_localTranforms[jointIndex];
    Matrix4 globalJointTransform = Matrix4();
    int currentJointParentIndex = GetJointParent(jointIndex);
    if (currentJointParentIndex != -1)
    {
        Matrix4 parent = m_currentPose.m_localTranforms[currentJointParentIndex];
        globalJointTransform = MultiplyMatrix4ByMatrix4(currentJointTransform, parent);
        GetJointCurrentPoseGlobalTransform(currentJointParentIndex);
    }
    return globalJointTransform;
}

//*******************************************************************
Matrix4 SkeletonInstance::GetJointBindPoseGlobalTransform(unsigned int jointIndex) const
{
    Matrix4 currentJointTransform = m_skeleton->m_jointTransforms[jointIndex];
    Matrix4 globalJointTransform = Matrix4();
    int currentJointParentIndex = GetJointParent(jointIndex);
    if (currentJointParentIndex != -1)
    {
        Matrix4 parent = m_skeleton->m_jointTransforms[currentJointParentIndex];
        globalJointTransform = MultiplyMatrix4ByMatrix4(currentJointTransform, parent);
        GetJointCurrentPoseGlobalTransform(currentJointParentIndex);
    }
    return globalJointTransform;
}
//*******************************************************************
int SkeletonInstance::GetJointParent(unsigned int jointIndex) const
{
    return m_skeleton->GetParentIndex(jointIndex);
}
