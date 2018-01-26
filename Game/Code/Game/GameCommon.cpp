#include "Game/GameCommon.hpp"

float ORHTO_WIDTH = 0.0f;
float ORHTO_HEIGHT = 0.0f;

float WINDOW_WIDTH = 0.0f;
float WINDOW_HEIGHT = 0.0f;
char const* WINDOW_TITLE = "";

float REFERENCE_HOST_TIME = 0.0f;
float REFERENCE_CLIENT_TIME = 0.0f;
int NUM_ALIEN_SPLITS = 2;

InputSystem* g_inputSystem = nullptr;
Camera2D* g_camera2D = nullptr;
Game* g_theGame = nullptr;
AudioSystem* g_theAudioSystem = nullptr;
SimpleRenderer* g_theSimpleRenderer = nullptr;
Console* g_theConsole = nullptr;
Renderer* g_theRenderer = nullptr;
Stopwatch* g_theStopwatch = nullptr;
