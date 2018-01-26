#pragma once
#include "Engine/Math/Matrix4.hpp"
#include <string>
#include <vector>

class Skeleton
{
public:
    Skeleton();
    ~Skeleton();

    // Reset the skeleton - clear out all bones that make up
    // this skeleton
    void Clear();

    // Adds a joint.  Can be parented to another 
    // joint within this skeleton.
    void AddJoint(char const *name, char const *parentName, Matrix4 const& transform);
    void AddParentIndice(std::string parentName);

    // get number of joints/bones in this skeleton.
    unsigned int GetJointCount() const;

    // Get a joint index by name, returns
    // (uint)(-1) if it doesn't exist.
    unsigned int GetJointIndex(char const *name);

    std::string GetJointName(unsigned int jointIndex);

    // Get the global transform for a joint.
    Matrix4 GetJointTransform(unsigned int jointIndex) const;
    Matrix4 GetJointTransform(char const *name) const;

    int GetParentIndex(unsigned int jointIndex) const;
    Matrix4 GetParentJointTransform(unsigned int jointIndex) const;
    std::string GetParentJointName(unsigned int jointIndex) const;

    void FlipX();

    //Skeleton, needs string names
    std::vector<std::string> m_jointNames;
    std::vector<std::string> m_parentJointNames;
    std::vector<int> m_parentJointIndices;
    std::vector<Matrix4> m_jointTransforms;
};