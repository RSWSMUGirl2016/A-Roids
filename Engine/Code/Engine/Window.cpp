#include "Engine/Window.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Profiler/Profiler.hpp"
#include "Game/GameCommon.hpp"
#include <sstream>

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);

#define WNDCLASS_GAME_NAME    ("GameWindowClass")

size_t Window::instanceCount = 0;

//*******************************************************************
static HINSTANCE GetCurrentHINSTANCE()
{
	return ::GetModuleHandle(NULL);
}

//*******************************************************************
LRESULT CALLBACK Window::EngineWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	Window* window = (Window*) ::GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if ((window != nullptr) && (window->m_windowMessageHandler != nullptr))
	{
		bool stop = window->m_windowMessageHandler(hwnd, message, wparam, lparam);
		if (stop)
		{
			return 0;
		}
	}


	switch (message)
	{
	case WM_CREATE:
		{
			CREATESTRUCT *cp = (CREATESTRUCT*)lparam;
			window = (Window*)cp->lpCreateParams;
			window->m_hwnd = hwnd;
			SetWindowLongPtr(window->m_hwnd, GWLP_USERDATA, (LONG_PTR)window);
		}
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
	
}

//*******************************************************************
static RECT WindowGetRect(int width, int height, DWORD style)
{
	RECT window_rect;
	window_rect.left = 0;
	window_rect.top = 0;
	window_rect.right = width;
	window_rect.bottom = height;

	// Adjust the size so the window is big enough with this style to allow the client
	// to be the size of window_rect
	::AdjustWindowRectEx(&window_rect, style, FALSE, WS_EX_APPWINDOW);

	return window_rect;
}

bool Window::RegisterGameWindowClass()
{
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(wc));

	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;

	wc.lpfnWndProc = EngineWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetCurrentHINSTANCE();

	wc.hIcon = NULL;
	wc.hIconSm = NULL;

	wc.hCursor = ::LoadCursorA(NULL, (LPCSTR)IDC_ARROW);
	wc.hbrBackground = (HBRUSH)0;
	wc.lpszMenuName = NULL;

	wc.lpszClassName = (LPCWSTR)WNDCLASS_GAME_NAME;

	ATOM result = RegisterClassEx(&wc);
	if (NULL == result) {
		DebuggerPrintf("Failed to register window class [ErrNo: %u]", GetLastError());
		return false;
	}
	instanceCount++;

	return true;
}

//*******************************************************************
void Window::UnregisterGameWindowClass()
{
	UnregisterClass((LPCTSTR)WNDCLASS_GAME_NAME, GetCurrentHINSTANCE());
}

//*******************************************************************
HWND Window::CreateTheWindow(char const *title, int x, int y, int resolutionX, int resolutionY)
{
	DWORD extended_style = 0;
	DWORD style = 0U;

	style |= (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_BORDER);

	style |= WS_VISIBLE;

	RECT win_rect = WindowGetRect(resolutionX, resolutionY, style);

	x += win_rect.left;
	y += win_rect.top;
	int w = win_rect.right - win_rect.left;
	int h = win_rect.bottom - win_rect.top;

	WCHAR wndName[1024];
	MultiByteToWideChar(GetACP(), 0, title, -1, wndName, sizeof(wndName) / sizeof(wndName[0]));	

	HWND hwnd = CreateWindowEx(extended_style, (LPCWSTR)WNDCLASS_GAME_NAME, wndName, style, x, y, w, h, NULL, NULL, GetCurrentHINSTANCE(), this);

	if (NULL == hwnd) {
		DWORD error = GetLastError();
		DebuggerPrintf("Failed to create window:  Error[%u]", error);
	}
	m_hwnd = hwnd;

	return hwnd;
}


//*******************************************************************
Window::Window()
	:m_gameConfig(GameConfig()),
	m_customWidth(0),
	m_customHeight(0)
{
	if (instanceCount == 0)
	{
		RegisterGameWindowClass();
	}

}

//*******************************************************************
Window::~Window()
{
	Close();

	if (instanceCount != 0)
	{
		--instanceCount;
		if (instanceCount == 0)
		{
			UnregisterGameWindowClass();
		}
	}
}

//*******************************************************************
void Window::SetClientSize(unsigned int width, unsigned int height)
{
	m_customWidth = width;
	m_customHeight = height;
}

//*******************************************************************
bool Window::Open()
{
	int leftPosition = 100;
	int topPosition = 100;

	//std::string wndwTitle;
	//m_gameConfig.GetWindowTitle(wndwTitle);
	int windowResolutionX = 0;
	//m_gameConfig.GetWindowResolutionX(windowResolutionX);
	int windowResolutionY = 0;
	//m_gameConfig.GetWindowResolutionY(windowResolutionY);
	//m_gameConfig.ListMembers();

	if (m_customWidth != 0)
	{
		windowResolutionX = (int)WINDOW_WIDTH;
	}

	if (m_customHeight != 0)
	{
		windowResolutionY = (int)WINDOW_HEIGHT;
	}

	char const *cWndwTitle = WINDOW_TITLE;
	HWND hwnd = CreateTheWindow(cWndwTitle, leftPosition, topPosition, windowResolutionX, windowResolutionY);
	m_hwnd = hwnd;

	return true;
}

//*******************************************************************
void Window::Close()
{
	::DestroyWindow(m_hwnd);
	UnregisterGameWindowClass();
}

//*******************************************************************
unsigned int Window::ProcessMessages()
{
    //PROFILE_LOG_SCOPE("ProcessMessages");
	int messagesProcessed = 0;
	if (NULL != m_hwnd)
	{
		MSG message;
		while (::PeekMessage(&message, m_hwnd, 0, 0, PM_REMOVE))
		{
			++messagesProcessed;
			::TranslateMessage(&message);
			::DispatchMessage(&message);
		}
	}
	return messagesProcessed;
}

//*******************************************************************
bool Window::IsOpen() const
{
	bool isOpen = (NULL != m_hwnd);
	return isOpen;
}

//*******************************************************************
bool Window::IsClosed() const
{
	bool isOpen = (NULL == m_hwnd);
	return isOpen;
}

//*******************************************************************
void Window::SetWindowTitle(const std::wstring& title)
{
	//std::string sTitle((const char*)&title[0], sizeof(wchar_t) / sizeof(char)*title.size());
	std::string sTitle(title.begin(), title.end());
	//m_gameConfig.SetWindowTitle(sTitle);
	::SetWindowText(m_hwnd, &title[0]);
}