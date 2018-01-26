#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <gl/GL.h>

#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Game/App.hpp"
#include "Engine/RHI/RHITypes.hpp"
#include "Engine/RHI/Sampler.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vertex3.hpp"
#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/Console.hpp"
#include "Engine/Threading/Thread.hpp"
#include "Engine/Profiler/Profiler.hpp"
#include "Engine/Memory/BuildConfig.hpp"
#include "Engine/Job/Job.hpp"
#include "Engine/Tools/fbx.h"
#include "Engine/Net/Net.hpp"
#include "Engine/Net/RemoteCommandService.hpp"

#include <math.h>
#include <cassert>
#include <crtdbg.h>
#include <string>

//*******************************************************************
HDC g_displayDeviceContext = nullptr;

//*******************************************************************
bool WindowsMessageHandlingProcedure(HWND windowHandle, UINT wmMessageCode, WPARAM wParam, LPARAM lParam)
{
    UNUSED(lParam);
    UNUSED(windowHandle);

    unsigned char keyCode = (unsigned char)wParam;
    switch (wmMessageCode)
    {
    case WM_CLOSE:
    case WM_DESTROY:
    case WM_QUIT:
    {
        g_theApp->OnExitRequested();
        return true;
    }
    case WM_SETFOCUS:
    {
        return true;
    }

    case WM_KILLFOCUS:
    {
        return true;
    }

    case WM_KEYDOWN:
    {
        g_theApp->OnKeyDown(keyCode);
        g_theApp->m_keyCode = keyCode;
        g_theApp->m_isKeyDown = true;
        return true;
    }

    case WM_KEYUP:
    {
        g_theApp->OnKeyUp(keyCode);
        g_theApp->m_isKeyDown = false;
        return true;
    }

    case WM_LBUTTONDOWN:
    {
        g_theApp->OnKeyDown(MOUSE_LEFTBTTN);
        return true;
    }

    case WM_RBUTTONDOWN:
    {
        g_theApp->OnKeyDown(MOUSE_RIGHTBTTN);
        return true;
    }

    case WM_LBUTTONUP:
    {
        g_theApp->OnKeyUp(MOUSE_LEFTBTTN);
        return true;
    }

    case WM_RBUTTONUP:
    {
        g_theApp->OnKeyUp(MOUSE_RIGHTBTTN);
        return true;
    }

    }


    return false;
}

//*******************************************************************
void Initialize(HINSTANCE applicationInstanceHandle)
{
    UNUSED(applicationInstanceHandle);
    SetProcessDPIAware();

    g_theApp = new App();
    LogStartup("Data/Log/game.log");
    JobSystemStartup(JOB_TYPE_COUNT);

    if (PROFILED_BUILD)
    {
        g_profiler = new Profiler();
        g_profiler->ProfilerSystemStartup();
    }

    ORHTO_WIDTH = 1280.0f;
    ORHTO_HEIGHT = 720.0f;
    WINDOW_WIDTH = ORHTO_WIDTH;
    WINDOW_HEIGHT = ORHTO_HEIGHT;
    WINDOW_TITLE = "A* 'ROIDS";

    g_theSimpleRenderer->SetUp(WindowsMessageHandlingProcedure);
    g_theGame->SetUp();

    g_theApp->m_cursorY = (int)ORHTO_HEIGHT - g_theSimpleRenderer->m_font->m_lineHeight;
}

//*******************************************************************
void Shutdown()
{
    if (PROFILED_BUILD)
    {
        g_profiler->ProfilerSystemShutdown();
        delete g_profiler;
        g_profiler = nullptr;
    }

    JobSystemShutdown();
    LogShutdown();
    CopyAndRenameLogFile("../Run_Win32/Data/Log/", "game");

    delete g_theApp;
    g_theApp = nullptr;
}

//*******************************************************************
void PrintReport()
{
    if (PROFILED_BUILD)
    {
        ProfilerTree* frame = g_profiler->ProfilerGetPreviousFrameTree();
        ProfilerReport report(frame);

        report.CreateTreeView();
        report.SortTreeByTotalTime();
        report.LogTreeView();
    }
}

//*******************************************************************
int WINAPI WinMain(HINSTANCE applicationInstanceHandle, HINSTANCE, LPSTR commandLineString, int)
{
    UNUSED(commandLineString);
    Initialize(applicationInstanceHandle);

    RemoteCommandService* rcs = new RemoteCommandService();
    UNUSED(rcs);

    while (!g_theApp->IsQuitting())
    {
        g_theSimpleRenderer->ProcessMessages();
        g_theApp->RunFrame();
    }

    Shutdown();

    return 0;
}

