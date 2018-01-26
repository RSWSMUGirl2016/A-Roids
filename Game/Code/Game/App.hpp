#pragma once
#include "Engine/Console.hpp"
#include "Engine/RHI/RHI.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Engine/RHI/ConstantBuffer.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/ParticleEffects/ParticleEmitter.hpp"

class App
{
public:
	App();
	~App();
public:
	void RunFrame();
	RHIOutput* GetOutput();
	void OnKeyDown(int keyCode);
	void OnKeyUp(int keyCode);
	void OnExitRequested();
	bool IsQuitting();
private:
	void Update(float deltaSeconds);        
    void Render() const;  
    void WrapAround();
public:
    float GetDeltaSeconds();
    //void LoadTextures();
public:
	bool m_isQuitting;
public:
    std::vector<console_message> m_consoleMessages;
    int m_keyCode;
    bool m_isKeyDown;
    bool m_wasEnterPressed;
    bool m_isDeveloperConsoleOpen;
    bool m_didDeveloperWindowJustOpen;
    bool m_isProfilerOpen;
    bool m_didProfilerJustOpen;

    bool m_drawCursor;
    int m_cursorX;
    int m_cursorY;
    bool m_treeView = true;

    Texture2D* m_cubeTexture;
};

extern App* g_theApp;