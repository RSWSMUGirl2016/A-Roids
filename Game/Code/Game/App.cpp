#include "Engine/Core/Camera2D.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/App.hpp"
#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Engine/RHI/VertexBuffer.hpp"
#include "Engine/Profiler/Profiler.hpp"
#include "Engine/Memory/Memory.hpp"

//*******************************************************************
App* g_theApp = nullptr;

//*******************************************************************
App::App()
	: m_isQuitting(false)
    , m_isDeveloperConsoleOpen(false)
    , m_didDeveloperWindowJustOpen(false)
    , m_isProfilerOpen(false)
    , m_didProfilerJustOpen(false)
    , m_keyCode(0)
    , m_isKeyDown(false)
    , m_drawCursor(false)
    , m_cursorX(5)
    , m_cursorY(0)
    , m_wasEnterPressed(false)
    , m_cubeTexture(nullptr)
{
	g_theSimpleRenderer = new SimpleRenderer();
	g_inputSystem = new InputSystem();
	g_camera2D = new Camera2D();
	g_theGame = new Game();
	g_theAudioSystem = new AudioSystem();
    g_theStopwatch = new Stopwatch();
}

//*******************************************************************
App::~App()
{
    delete g_theAudioSystem;
    g_theAudioSystem = nullptr;

    delete g_theGame;
    g_theGame = nullptr;

    delete g_camera2D;
    g_camera2D = nullptr;

	delete g_inputSystem;
	g_inputSystem = nullptr;

	delete g_theSimpleRenderer;
	g_theSimpleRenderer = nullptr;

    delete  g_theStopwatch;
    g_theStopwatch = nullptr;
}

//*******************************************************************
void App::RunFrame()
{	
	g_theSimpleRenderer->Present();    
    g_theSimpleRenderer->Update(GetDeltaSeconds());
 	Update(GetDeltaSeconds());    

    if(m_isQuitting)
        return;

	Render();
    WrapAround();
    ProfileMemoryFrameTick();
}

//*******************************************************************
RHIOutput* App::GetOutput()
{
	return g_theSimpleRenderer->m_output;
}

//*******************************************************************
void App::OnKeyDown(int keyCode)
{
    g_inputSystem->OnKeyDown(keyCode);
}

//*******************************************************************
void App::OnKeyUp(int keyCode)
{
	g_inputSystem->OnKeyUp(keyCode);
}

//*******************************************************************
void App::OnExitRequested()
{
	m_isQuitting = true;
}

//*******************************************************************
bool App::IsQuitting()
{
	return m_isQuitting;
}

//*******************************************************************
void App::Update(float deltaSeconds)
{
    m_drawCursor = !m_drawCursor;
    g_theAudioSystem->Update();

    if (g_theGame != nullptr)
	    g_theGame->Update(deltaSeconds);	

    g_inputSystem->Update();
}

//*******************************************************************
void App::Render() const
{
    if (g_theGame != nullptr)
        g_theGame->Render();
}

//*******************************************************************
void App::WrapAround() 
{
    if (g_theGame != nullptr)
        g_theGame->WrapAround();
}

//*******************************************************************
float App::GetDeltaSeconds()
{
	double timeNow = GetCurrentTimeSeconds();
	static double lastFrameTime = timeNow;
	double deltaSeconds = timeNow - lastFrameTime;

	// Wait until [nearly] the minimum frame time has elapsed (limit frame rate to within the max)
	while (deltaSeconds < MIN_SECONDS_PER_FRAME * .999f)
	{
		timeNow = GetCurrentTimeSeconds();
		deltaSeconds = timeNow - lastFrameTime;
	}
	lastFrameTime = timeNow;

	// Clamp deltaSeconds to be within the max time allowed (e.g. sitting at a debug break point)
	if (deltaSeconds > MAX_SECONDS_PER_FRAME)
	{
		deltaSeconds = MAX_SECONDS_PER_FRAME;
	}

	return (float)deltaSeconds;
}


