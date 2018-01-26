#include "Engine/Input/InputSystem.hpp"

//*******************************************************************
InputSystem::InputSystem()
	: m_doesAppHaveFocus(true)
	, m_hideMouseCursorWhenFocused(false)
{
	for (int xboxControllerIndex = 0; xboxControllerIndex < NUM_CONTROLLERS; ++xboxControllerIndex)
	{
		m_controllers[xboxControllerIndex] = XboxController();
	}

	for (int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex)
	{
		m_keyStates[keyIndex].m_isDown = false;
		m_keyStates[keyIndex].m_wasDown = false;
	}
}

//*******************************************************************
InputSystem::~InputSystem()
{
}

//*******************************************************************
void InputSystem::Update()
{
	if (!DoesAppHaveFocus())
		Sleep(100);
	
	UpdateControllers();
	UpdateKeyboard();
	//RunMessagePump();
}

//*******************************************************************
void InputSystem::OnKeyDown(int keyCode)
{
	m_keyStates[keyCode].m_isDown = true;
}

//*******************************************************************
void InputSystem::OnKeyUp(int keyCode)
{
	m_keyStates[keyCode].m_isDown = false;
}

//*******************************************************************
void InputSystem::OnAppGainedFocus()
{
	m_doesAppHaveFocus = true;
	if (m_hideMouseCursorWhenFocused)
	{
		HideMouseCursor();
	}		
}

//*******************************************************************
void InputSystem::OnAppLostFocus()
{
	m_doesAppHaveFocus = false;
	if (m_hideMouseCursorWhenFocused)
	{
		ShowMouseCursor();
	}		
}

//*******************************************************************
bool InputSystem::DoesAppHaveFocus() const
{
	return m_doesAppHaveFocus;
}

//*******************************************************************
Vector2 InputSystem::GetMouseScreenCoords() const
{
	POINT cursorScreenPosInts;
	GetCursorPos(&cursorScreenPosInts);
	Vector2 cursorScreenPosFloats((float)cursorScreenPosInts.x, (float)cursorScreenPosInts.y);
	return cursorScreenPosFloats;
}

//*******************************************************************
void InputSystem::SetMouseCursorPosition(const Vector2& newMouseScreenCoords)
{
	int x = (int)newMouseScreenCoords.x;
	int y = (int)newMouseScreenCoords.y;
	SetCursorPos(x, y);
}

//*******************************************************************
Vector2 InputSystem::GetScreenSize()
{
	HWND desktopWindowHandle = GetDesktopWindow();
	RECT desktopRect;
	GetWindowRect(desktopWindowHandle, &desktopRect);

	Vector2 screenSize;
	screenSize.x = (float)(desktopRect.right - desktopRect.left);
	screenSize.y = (float)(desktopRect.bottom - desktopRect.top);
	return screenSize;
}

//*******************************************************************
void InputSystem::HideMouseCursor()
{
	for (;;)
	{
		int newCursorShowValue = ShowCursor(FALSE);
		if (newCursorShowValue < 0)
			break;
	}
	
}

//*******************************************************************
void InputSystem::ShowMouseCursor()
{
	for (;;)
	{
		int newCursorShowValue = ShowCursor(TRUE);
		if (newCursorShowValue >= 0)
			break;
	}
}

//*******************************************************************
void InputSystem::SetMouseCursorHiddenWhenFocus(bool hideMouseWhenNotFocused)
{
	m_hideMouseCursorWhenFocused = hideMouseWhenNotFocused;
	if (m_hideMouseCursorWhenFocused && m_doesAppHaveFocus)
		HideMouseCursor();
	else
		ShowMouseCursor();
}

//*******************************************************************
bool InputSystem::IsKeyDown(int keyCode)
{
	return m_keyStates[keyCode].m_isDown;
}

//*******************************************************************
bool InputSystem::WasKeyJustPressed(int keyCode)
{
	return ((m_keyStates[keyCode].m_isDown == false) && (m_keyStates[keyCode].m_wasDown == true));
}

//*******************************************************************
const XboxController& InputSystem::GetControllerState(int controllerID)
{
	return m_controllers[controllerID];
}

//*******************************************************************
void InputSystem::UpdateControllers()
{
	for (int xboxControllerIndex = 0; xboxControllerIndex < NUM_CONTROLLERS; ++xboxControllerIndex)
	{
		m_controllers[xboxControllerIndex].Update();
		m_controllers[xboxControllerIndex].m_controllerID = xboxControllerIndex;
	}
}

//*******************************************************************
void InputSystem::UpdateKeyboard()
{
	for (int keyIndex = 0; keyIndex < NUM_KEYS; ++keyIndex)
	{
		m_keyStates[keyIndex].m_wasDown = m_keyStates[keyIndex].m_isDown;
	}
}

//*******************************************************************
void InputSystem::RunMessagePump()
{
	MSG queuedMessage;
	for (;; )
	{
		const BOOL wasMessagePresent = PeekMessage(&queuedMessage, NULL, 0, 0, PM_REMOVE);
		if (!wasMessagePresent)
		{
			break;
		}

		TranslateMessage(&queuedMessage);
		DispatchMessage(&queuedMessage);
	}
}
