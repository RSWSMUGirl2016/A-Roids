#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/MeshBuilder/Skeleton.hpp"

//*******************************************************************
Skeleton::Skeleton()
{
}

//*******************************************************************
Skeleton::~Skeleton()
{
}

//*******************************************************************
void Skeleton::Clear()
{
    m_jointNames.clear();
    m_parentJointNames.clear();
    m_jointTransforms.clear();
}

//*******************************************************************
void Skeleton::AddJoint(char const *name, char const *parentName, Matrix4 const& transform)
{
    std::string newName;
    std::string newParentName;
    if (name == nullptr)
    {
        newName = "";
    }
    else
    {
        newName = name;
    }

    if (parentName == nullptr)
    {
        newParentName = "";
    }
    else
    {
        newParentName = parentName;
    }
    

    m_jointNames.push_back(newName);
    m_parentJointNames.push_back(newParentName);
    m_jointTransforms.push_back(transform);
    AddParentIndice(newParentName);
}

//*******************************************************************
void Skeleton::AddParentIndice(std::string parentName)
{
    if (parentName == "")
    {
        m_parentJointIndices.push_back(-1);
    }

    for (int jointIndex = 0; jointIndex < (int)m_jointNames.size(); jointIndex++)
    {
        if (parentName == m_jointNames[jointIndex])
        {
            m_parentJointIndices.push_back(jointIndex);
        }
    }
}

//*******************************************************************
unsigned int Skeleton::GetJointCount() const
{
    return m_jointNames.size();
}

//*******************************************************************
unsigned int Skeleton::GetJointIndex(char const *name)
{
    int index = std::find(m_jointNames.begin(), m_jointNames.end(), name) - m_jointNames.begin();
    return (unsigned int)index;
}

//*******************************************************************
std::string Skeleton::GetJointName(unsigned int jointIndex)
{
    return m_jointNames[jointIndex];
}

//*******************************************************************
Matrix4 Skeleton::GetJointTransform(unsigned int jointIndex) const
{
    if (jointIndex == -1)
    {
        return Matrix4();
    }
    return m_jointTransforms[jointIndex];
}

//*******************************************************************
Matrix4 Skeleton::GetJointTransform(char const *name) const
{
    int index = std::find(m_jointNames.begin(), m_jointNames.end(), name) - m_jointNames.begin();
    return GetJointTransform((unsigned int)index);
}

//*******************************************************************
int Skeleton::GetParentIndex(unsigned int jointIndex) const
{
    return m_parentJointIndices[jointIndex];
}

//*******************************************************************
Matrix4 Skeleton::GetParentJointTransform(unsigned int jointIndex) const
{
    int parentIndex = GetParentIndex(jointIndex);
    if (parentIndex == -1)
    {
        ASSERT_OR_DIE(true, "Joint doesn't have a parent, therefore I should not be called");
    }
    return GetJointTransform(parentIndex);
}

//*******************************************************************
std::string Skeleton::GetParentJointName(unsigned int jointIndex) const
{
    std::string parentName = m_parentJointNames[jointIndex];    
    return parentName;
}

//*******************************************************************
void Skeleton::FlipX()
{
    for (int jointTransformIndex = 0; jointTransformIndex < (int)m_jointTransforms.size(); jointTransformIndex++)
    {
        m_jointTransforms[jointTransformIndex].TransformDirection(Vector3(-1.f, 0.f, 0.f));
    }
}
