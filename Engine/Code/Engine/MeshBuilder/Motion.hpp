#pragma once
#include "Engine/MeshBuilder/Pose.hpp"
#include <string>
#include <vector>

class Motion
{
public:
    std::string m_name;
    float m_framerate;
    std::vector<Pose*> m_poses;

    // Make sure poses is exactly long enough to encompass this time.
    float SetDuration(float time);
    // framerate is 10hz, that means the time between two frames takes 1/10th of a second.
    // therefore if we have 3 frames in this motion, the total duration would be 
    // (3 - 1) * 0.1,= or 0.2 seconds long.  
    float GetDuration() const { return (m_poses.size() - 1) / m_framerate; };
    unsigned int GetFrameCount() const { return m_poses.size(); };
    // This will fill out a pose with the evaluated pose at time
    // left as an excercise...
    void Evaluate(Pose& out, float time) const;

    Pose* GetPose(unsigned int frameIndex);
};