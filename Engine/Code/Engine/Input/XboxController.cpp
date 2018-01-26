#include <Windows.h>
#include <Xinput.h>
#pragma comment( lib, "xinput9_1_0" )
#include "Engine/Input/XboxController.hpp"
#include "Engine/Math/MathUtilities.hpp"

//*******************************************************************
//Credit to Squirrel Eiserloh
XboxController::XboxController()
	: m_controllerID(-1)
	, m_isControllerConnected(false)
{
	for (int buttonIndex = 0; buttonIndex < NUM_BUTTONS; ++buttonIndex)
	{
		m_buttons[buttonIndex].m_isPressed = false;
		m_buttons[buttonIndex].m_wasPressed = false;
	}
}

//*******************************************************************
XboxController::~XboxController()
{
}

//*******************************************************************
void XboxController::Update()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_controllerID, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		m_isControllerConnected = false;
		return;
	}
	m_isControllerConnected = true;

	for (int buttonIndex = 0; buttonIndex < NUM_BUTTONS; ++buttonIndex)
	{
		m_buttons[buttonIndex].m_wasPressed = m_buttons[buttonIndex].m_isPressed;
	}

	m_buttons[BUTTON_A].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0;
	m_buttons[BUTTON_B].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0;
	m_buttons[BUTTON_X].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0;
	m_buttons[BUTTON_Y].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0;
	m_buttons[BUTTON_START].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_START) != 0;
	m_buttons[BUTTON_BACK].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK) != 0;
	m_buttons[BUTTON_TOP_LEFT].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0;
	m_buttons[BUTTON_TOP_RIGHT].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	m_buttons[BUTTON_TOP_RIGHT].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0;
	m_buttons[BUTTON_DPAD_UP].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0;
	m_buttons[BUTTON_DPAD_RIGHT].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0;
	m_buttons[BUTTON_DPAD_DOWN].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0;
	m_buttons[BUTTON_DPAD_LEFT].m_isPressed = (xboxControllerState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0;
	//m_buttons[BUTTON_TOP_RIGHT].m_isPressed = (xboxControllerState.Gamepad.bRightTrigger & VK_PAD_RTRIGGER) != 0;

	UpdateLeftTrigger();
	UpdateRightTrigger();

	UpdateLeftJoystickValues();
	UpdateRightJoystickValues();
}

//*******************************************************************
bool XboxController::IsButtonPressed(ButtonType buttonType)
{
	return m_buttons[buttonType].m_isPressed;
}

//*******************************************************************
bool XboxController::WasButtonPressed(ButtonType buttonType)
{
	return ((m_buttons[buttonType].m_isPressed == false) && (m_buttons[buttonType].m_wasPressed == true));
}

//*******************************************************************
bool XboxController::IsControlledConnected()
{
	return m_isControllerConnected;
}

//*******************************************************************
void XboxController::RumbleController(float leftmotor, float rightmotor)
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_controllerID, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		m_isControllerConnected = false;
		return;
	}
	m_isControllerConnected = true;


	//#HELP_FOR_SCREEN_SHAKE
	//http://www.david-amador.com/2012/04/xbox-360-controller-input-in-c-via-xinput/
	XINPUT_VIBRATION Vibration;

	memset(&Vibration, 0, sizeof(XINPUT_VIBRATION));

	int leftVib = (int)(leftmotor*65535.0f);
	int rightVib = (int)(rightmotor*65535.0f);

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = (WORD)leftVib;
	Vibration.wRightMotorSpeed = (WORD)rightVib;
	// Vibrate the controller
	XInputSetState((int)m_controllerID, &Vibration);
}

//*******************************************************************
void XboxController::UpdateLeftTrigger()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_controllerID, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		m_isControllerConnected = false;
		return;
	}
	m_isControllerConnected = true;

	if (xboxControllerState.Gamepad.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		m_buttons[BUTTON_TOP_LEFT].m_isPressed = false;
	}
	else
	{
		m_buttons[BUTTON_TOP_LEFT].m_isPressed = true;
	}
}


//*******************************************************************
void XboxController::UpdateRightTrigger()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_controllerID, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		m_isControllerConnected = false;
		return;
	}
	m_isControllerConnected = true;

	if (xboxControllerState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
	{
		m_buttons[BUTTON_TOP_RIGHT].m_isPressed = false;
	}
	else
	{
		m_buttons[BUTTON_TOP_RIGHT].m_isPressed = true;
	}
}

//*******************************************************************
void XboxController::UpdateLeftJoystickValues()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_controllerID, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		m_isControllerConnected = false;
		return;
	}
	m_isControllerConnected = true;

	const float MIN_MAGNITUDE = (float)XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE / (float)MAXIMUM_JOYSTICK_REACH;
	const float MAX_MAGNITUDE = .95f;

	Vector2& pos = m_leftJoyStick.m_position;
	float& magnitude = m_leftJoyStick.m_magnitude;
	float& angle = m_leftJoyStick.m_angleDegrees;

	pos.x = (float)xboxControllerState.Gamepad.sThumbLX / (float)MAXIMUM_JOYSTICK_REACH;
	pos.y = (float)xboxControllerState.Gamepad.sThumbLY / (float)MAXIMUM_JOYSTICK_REACH;
	angle = pos.CalcHeadingDegrees();
	magnitude = pos.CalcLength();

	if (magnitude > MAX_MAGNITUDE)
		magnitude = MAX_MAGNITUDE;

	if (magnitude < MIN_MAGNITUDE)
		magnitude = MIN_MAGNITUDE;

	magnitude = RangeMapFloat(magnitude, MIN_MAGNITUDE, MAX_MAGNITUDE, 0.f, 1.f);

	pos.x = magnitude * CosDegrees(angle);
	pos.y = magnitude * SinDegrees(angle);
}

//*******************************************************************
void XboxController::UpdateRightJoystickValues()
{
	XINPUT_STATE xboxControllerState;
	memset(&xboxControllerState, 0, sizeof(xboxControllerState));
	DWORD errorStatus = XInputGetState(m_controllerID, &xboxControllerState);
	if (errorStatus != ERROR_SUCCESS)
	{
		m_isControllerConnected = false;
		return;
	}
	m_isControllerConnected = true;

	const float MIN_MAGNITUDE = (float)XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE / (float)MAXIMUM_JOYSTICK_REACH;
	const float MAX_MAGNITUDE = .95f;

	Vector2& pos = m_rightJoyStick.m_position;
	float& magnitude = m_rightJoyStick.m_magnitude;
	float& angle = m_rightJoyStick.m_angleDegrees;

	pos.x = (float)xboxControllerState.Gamepad.sThumbRX / (float)MAXIMUM_JOYSTICK_REACH;
	pos.y = (float)xboxControllerState.Gamepad.sThumbRY / (float)MAXIMUM_JOYSTICK_REACH;
	angle = pos.CalcHeadingDegrees();
	magnitude = pos.CalcLength();

	if (magnitude > MAX_MAGNITUDE)
		magnitude = MAX_MAGNITUDE;

	if (magnitude < MIN_MAGNITUDE)
		magnitude = MIN_MAGNITUDE;

	magnitude = RangeMapFloat(magnitude, MIN_MAGNITUDE, MAX_MAGNITUDE, 0.f, 1.f);

	pos.x = magnitude * CosDegrees(angle);
	pos.y = magnitude * SinDegrees(angle);
}
