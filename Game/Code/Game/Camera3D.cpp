#include "Game/Camera3D.hpp"
#include "Engine/Math/MathUtilities.hpp"

//*******************************************************************
Camera3D::Camera3D()
	: m_position(0.f, 0.f, 0.f)
	, m_yawDegreesAboutZ(0.f)
	, m_pitchDegreesAboutY(0.f)
	, m_rollDegreesAboutX(0.f)
{
}

//*******************************************************************
Camera3D::Camera3D(Vector3 position)
	: m_position(position)
	, m_yawDegreesAboutZ(0.f)
	, m_pitchDegreesAboutY(0.f)
	, m_rollDegreesAboutX(0.f)
{
}

//*******************************************************************
Camera3D::~Camera3D()
{
}

//For Forseth's Calculations
//x --> x, y --> z, z --> y
//*******************************************************************
Vector3 Camera3D::GetForwardXYZ() const
{
	//return Vector3(CosDegrees(m_yawDegreesAboutZ) * CosDegrees(m_pitchDegreesAboutY), SinDegrees(m_yawDegreesAboutZ) * CosDegrees(m_pitchDegreesAboutY), -1.f * SinDegrees(m_pitchDegreesAboutY));
    return Vector3(CosDegrees(m_pitchDegreesAboutY) * CosDegrees(m_yawDegreesAboutZ), SinDegrees(m_pitchDegreesAboutY) * CosDegrees(m_yawDegreesAboutZ), -1.f * SinDegrees(m_yawDegreesAboutZ));
}

//For Forseth's Calculations
//x --> x, y --> z, z --> y
//*******************************************************************
Vector3 Camera3D::GetForwardXY() const
{    
	//return Vector3(CosDegrees(m_yawDegreesAboutZ), SinDegrees(m_yawDegreesAboutZ), 0.f);
    return Vector3(CosDegrees(m_pitchDegreesAboutY), SinDegrees(m_pitchDegreesAboutY), 0.f);
}

//For Forseth's Calculations
//x --> x, y --> z, z --> y
//*******************************************************************
Vector3 Camera3D::GetLeftXY() const
{
	//return Vector3(-1.f*SinDegrees(m_yawDegreesAboutZ), CosDegrees(m_yawDegreesAboutZ), 0.f);
    return Vector3(-1.f*SinDegrees(m_pitchDegreesAboutY), CosDegrees(m_pitchDegreesAboutY), 0.f);
}

