#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "Game/GameConfig.hpp"

class Window
{
public:
	typedef bool(*WindowMessageHandler)(HWND, UINT, WPARAM, LPARAM);
	WindowMessageHandler m_windowMessageHandler;
	Window();
	~Window();
	void SetClientSize(unsigned int width, unsigned int height);
	static LRESULT CALLBACK EngineWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
	bool RegisterGameWindowClass();
	void UnregisterGameWindowClass();
	HWND CreateTheWindow(char const *title, int x, int y, int resolutionX, int resolutionY);
	bool Open();
	void Close();
	unsigned int ProcessMessages();
	bool IsOpen() const;
	bool IsClosed() const;

	void SetWindowTitle(const std::wstring& title);
public:
	HWND			m_hwnd;
	GameConfig		m_gameConfig;

private:
	int				m_customWidth;
	int				m_customHeight;
	static size_t   instanceCount;
};