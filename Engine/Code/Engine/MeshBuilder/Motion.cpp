#include "Engine/MeshBuilder/Motion.hpp"

#define UNUSED(x) (void)(x);

//*******************************************************************
float Motion::SetDuration(float time)
{
    float numFrames = time / (1.f / m_framerate);
    numFrames += 1;
    for (int numFramesIndex = 0; numFramesIndex < numFrames; numFramesIndex++)
    {
        m_poses.push_back(new Pose());
    }
    return numFrames;
}

//*******************************************************************
void Motion::Evaluate(Pose& out, float time) const
{
    float currentPoseTime = fmod(time, GetDuration());
    //get correct pose based on time    
    unsigned int currentPoseIndex = (unsigned int)floor(currentPoseTime * m_framerate);;
    out = *m_poses[currentPoseIndex];
    
    //do interpolation between poses
}

//*******************************************************************
Pose* Motion::GetPose(unsigned int frameIndex)
{
    return m_poses[frameIndex];
}
