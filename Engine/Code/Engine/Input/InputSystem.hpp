#pragma once
#include "Engine/Input/XboxController.hpp"

const int NUM_KEYS = 256;
const int NUM_CONTROLLERS = 4;

// Same as VK windows keys for now...
enum KeyCode
{
	KEY_ENTER = 0x0D,
	KEY_ESCAPE = 0X1B,
	KEY_UPARROW = 0X26,
	KEY_DOWNARROW = 0X28,
	KEY_F1 = 0X70,
	KEY_F2 = 0X71,
	KEY_F3 = 0X72,
	KEY_F4 = 0X73,
	KEY_F5 = 0X74,
	KEY_F6 = 0X75,
	KEY_F7 = 0X77,
	KEY_F8 = 0X78,
	KEY_F9 = 0X79,
	KEY_F10 = 0X7A,
	KEY_F11 = 0X7B,
	KEY_F12 = 0X7C,
	KEY_LEFTARROW = 0x25,
	KEY_RIGHTARROW = 0x27,
	KEY_SPACEBAR = 0x20,
	KEY_SHIFT = 0x10,
	MOUSE_LEFTBTTN = 0x01,
	MOUSE_RIGHTBTTN = 0x02,
    KEY_OEM3 = 0xC0, //Tilde
    KEY_BACK = 0x08,
    KEY_OEM2 = 0xBF, //Forward Slash
    KEY_PERIOD = 0xBE,
    KEY_QUOTE = 0xDE,
    KEY_COMMA = 0xBC,
    KEY_OEM1 = 0xBA,
    KEY_DASH = 0x07,
};

class KeyState
{
public:
	KeyState()
		: m_isDown(false)
		, m_wasDown(false)
	{};
	bool m_isDown;
	bool m_wasDown;

    bool m_wasJustPressed;
    bool m_wasJustReleased;
};

class InputSystem
{
public:
	bool m_hideMouseCursorWhenFocused;
	bool m_doesAppHaveFocus;
	KeyState m_keyStates[NUM_KEYS];
	XboxController m_controllers[NUM_CONTROLLERS];
public:
	InputSystem();
	~InputSystem();
public:
	void Update();
	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnAppGainedFocus();
	void OnAppLostFocus();
	bool DoesAppHaveFocus() const;
public:
	Vector2 GetMouseScreenCoords() const;
	void SetMouseCursorPosition(const Vector2& newMouseScreenCoords);
	Vector2 GetScreenSize();
	void HideMouseCursor();
	void ShowMouseCursor();
	void SetMouseCursorHiddenWhenFocus(bool hideMouseWhenNotFocused);
public:
	bool IsKeyDown(int keyCode);
	bool WasKeyJustPressed(int keyCode);
	const XboxController& GetControllerState(int controllerID);
private:
	void UpdateControllers();
	void UpdateKeyboard();
	void RunMessagePump();
};