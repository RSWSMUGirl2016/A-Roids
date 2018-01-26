#pragma once
#include <Windows.h>
#include <Xinput.h>
#pragma comment( lib, "xinput9_1_0" )
#include "Engine/Math/Vector2.hpp"

const int MAXIMUM_JOYSTICK_REACH = 32768;

enum ButtonType
{
	BUTTON_NONE = -1,
	BUTTON_A,
	BUTTON_B,
	BUTTON_X,
	BUTTON_Y,
	BUTTON_START,
	BUTTON_BACK,
	BUTTON_TOP_LEFT,
	BUTTON_TOP_RIGHT,
	BUTTON_BOTTOM_LEFT,
	BUTTON_BOTTOM_RIGHT,
	BUTTON_DPAD_UP,
	BUTTON_DPAD_RIGHT,
	BUTTON_DPAD_DOWN,
	BUTTON_DPAD_LEFT,
	NUM_BUTTONS
};

class XBoxButton
{
public:
	XBoxButton() { m_isPressed = false; m_wasPressed = false; m_buttonType = BUTTON_NONE; }
public:
	bool m_isPressed;
	bool m_wasPressed;
	ButtonType m_buttonType;
};

class XBoxJoyStick
{
public:
	XBoxJoyStick() { m_position = Vector2(); m_magnitude = 0.f; m_angleDegrees = 0.f; }
public:
	Vector2 m_position;
	float m_magnitude;
	float m_angleDegrees;
};

class XboxController
{
public:
	int m_controllerID;
	bool m_isControllerConnected;
	XBoxJoyStick m_leftJoyStick;
	XBoxJoyStick m_rightJoyStick;
	XBoxButton m_buttons[NUM_BUTTONS];
public:
	XboxController();
	~XboxController();
public:
	void Update();
	bool IsButtonPressed(ButtonType buttonType);
	bool WasButtonPressed(ButtonType buttonType);
	bool IsControlledConnected();
	void RumbleController(float leftmotor = 0.0f, float rightmotor = 0.0f);
	void UpdateLeftTrigger();
	void UpdateRightTrigger();
private:
	void UpdateLeftJoystickValues();
	void UpdateRightJoystickValues();	
};