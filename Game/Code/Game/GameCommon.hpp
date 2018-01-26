#pragma once
#include "Engine/Core/Camera2D.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Game/Game.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Window.hpp"
#include "Engine/Renderer/SimpleRenderer.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameConfig.hpp"
#include "Engine/Core/Stopwatch.hpp"
#include "Engine/Math/Vector2.hpp"

const float MIN_FRAMES_PER_SECOND = 10.f; // Game time will slow down beneath this frame rate
const float MAX_FRAMES_PER_SECOND = 60.f;
const float MIN_SECONDS_PER_FRAME = (1.f / MAX_FRAMES_PER_SECOND);
const float MAX_SECONDS_PER_FRAME = (1.f / MIN_FRAMES_PER_SECOND);

const float SPAWN_TIME_FOR_PLAYER = 0.5f;
const float JOIN_DISPALY_TEXT_FOR_PLAYER = 2.0f;
const float SHOT_RATE_FOR_PLAYER = 0.5f;
const float RELOAD_TIME_FOR_PLAYER = 0.25f;
const int NUM_SHOTS_UNTIL_PLAYER_DEATH = 10;
const float DESTROY_BULLET_TIME = 1.5f;
const float MIN_RADIUS_OF_ENEMY = 15.0f;

const float BULLET_LIFETIME_SECONDS = 1.0f;
const float MISSILE_LIFETIME_SECONDS = 5.0f;
const float MISSILE_VELOCITY = 75.0f;
const float MIN_RADIUS_OF_ASTEROID = 25.0f;
const Vector2 PLAYER_LIVES_POSITION = Vector2(25.f, 25.f);
const float TIME_ALIEN_IS_ON_SCREEN = 20.0f;
const float TIME_TO_SPAWN_NEW_MISSILE = 2.0f;

const int NEW_LIFE_MOD = 2;
const int NEW_STARBURST_MOD = 2;
const int BOSS_ASTEROID_MOD = 3;
const int NEW_ALIEN_MOD = 2;

extern float ORHTO_WIDTH;
extern float ORHTO_HEIGHT;

extern float WINDOW_WIDTH;
extern float WINDOW_HEIGHT;
extern char const* WINDOW_TITLE;

const int MAX_NUM_BUMPERS = 16;
const int MAX_NUM_BALLS = 1000;

const int MAX_NUM_LISTENER_SOCKETS = 10;

extern float REFERENCE_HOST_TIME;
extern float REFERENCE_CLIENT_TIME;
extern int NUM_ALIEN_SPLITS;

extern InputSystem* g_inputSystem;
extern Camera2D* g_camera2D;
extern Game* g_theGame;
extern AudioSystem* g_theAudioSystem;
extern SimpleRenderer* g_theSimpleRenderer;
extern Console* g_theConsole;
extern Renderer* g_theRenderer;
extern Stopwatch* g_theStopwatch;

//*******************************************************************
// Credit to Squirrel Eiserloh
#define UNUSED(x) (void)(x);