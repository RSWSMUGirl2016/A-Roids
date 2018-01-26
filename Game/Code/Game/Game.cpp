#include "Game/Game.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/Math2D.hpp"
#include "Engine/Math/MathUtilities.hpp"
#include "Game/App.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Memory/Memory.hpp"
#include "Engine/Memory/BuildConfig.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Threading/Thread.hpp"
#include "Engine/Profiler/Profiler.hpp"
#include "Engine/Job/Job.hpp"

#include <stdio.h>
#include <cstring>
#include <string>
#include <sstream>

using namespace std;

//*******************************************************************
int NUM_STARTING_ASTEROIDS = 4;
int NUM_BOSS_ASTEROIDS = 2;
int CURRENT_WAVE = 1;

//*******************************************************************
// Credit to Squirrel Eiserloh
Game::Game()
    : m_IsQuitting(false)
    , m_IsPaused(false)
    , m_numberOfLivesForPlayer(NUM_LIVES_FOR_PLAYER)
    , m_screenShake(0.f, 0.f)
    , m_playerHasStarBurst(false)
    , m_timeForScreenShake(0.5f)
    , m_currentTimeForScreenShake(0.0f)
    , m_timeForGameMusicToPlay(45.0f)
    , m_currentTimeForGameMusicToPlay(0.0f)
    , m_starfieldBackground(nullptr)
    , m_timeForFlickeringStarburstText(0.3f)
    , m_currentTimeForFlickeringStarburstText(0.0f)
    , m_displayStarburstText(true)
    , m_timeForFlickeringNewLifeText(0.3f)
    , m_currentTimeForFlickeringNewLifeText(0.0f)
    , m_displayNewLifeText(true)
    , m_currentGameState(START_GAME_STATE)
    , m_timeForFlickeringStartGameText(0.3f)
    , m_currentTimeForFlickeringStartGameText(0.0f)
    , m_displayStartGameText(true)
    , m_timeForFlickeringRestartGameText(0.3f)
    , m_currentTimeForFlickeringRestartGameText(0.0f)
    , m_displayRestartGameText(true)
    , m_score(0)
    , m_alien(nullptr)
    , m_currentTimeAlienExists(0.0f)
    , m_timeSinceLastMissileWasSpawned(0.0f)
{
    g_inputSystem->SetMouseCursorHiddenWhenFocus(true);
    g_theConsole = new Console();

    m_screenShake = Vector2();
    m_xboxController = XboxController();

    m_healthPickup = nullptr;

    m_starburst = nullptr;

    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        m_asteroids[asteroidIndex] = nullptr;
    }

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        m_bullets[bulletIndex] = nullptr;
    }

    for (int missileIndex = 0; missileIndex < MAX_BULLETS; ++missileIndex)
    {
        m_missiles[missileIndex] = nullptr;
    }

    for (int lifeIndex = 0; lifeIndex < NUM_LIVES_FOR_PLAYER; ++lifeIndex)
    {
        m_currentLives[lifeIndex] = nullptr;
    }
}


//*******************************************************************
Game::~Game()
{
    if (g_theConsole != nullptr)
    {
        delete g_theConsole;
        g_theConsole = nullptr;
    }

    DestroyShip();
    DestroyAllAsteroids();
    DestroyAllBullets();
    DestroyAllMissiles();
    DestroyAlien();
}

//*******************************************************************
void Game::SetUp()
{
    std::string spaceShipSpriteSheetName = "Data/Images/ShooterSpriteSheet_Modified.png";
    Texture2D* spaceShipSpriteSheetTexture = new Texture2D(g_theSimpleRenderer->m_device, spaceShipSpriteSheetName.c_str());
    m_spaceShipsSpriteSheet = new SpriteSheet(spaceShipSpriteSheetTexture, 18, 9);

    std::string asteroidSpriteSheetName = "Data/Images/asteroid.png";
    Texture2D* asteroidSpriteSheetTexture = new Texture2D(g_theSimpleRenderer->m_device, asteroidSpriteSheetName.c_str());
    m_asteroidsSpriteSheet = new SpriteSheet(asteroidSpriteSheetTexture, 1, 4);

    std::string alienSpriteSheetName = "Data/Images/AlienSprite.png";
    Texture2D* alienSpriteSheetTexture = new Texture2D(g_theSimpleRenderer->m_device, alienSpriteSheetName.c_str());
    m_alienSpriteSheet = new SpriteSheet(alienSpriteSheetTexture, 1, 2);

    //*******************************************************************
    //Player Texture Coords
    AABB2D newPlayerShipTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(2, 5);
    m_playerShipTextureCoords.m_mins = Vector2(newPlayerShipTextCoords.m_maxs.y, newPlayerShipTextCoords.m_maxs.x);
    m_playerShipTextureCoords.m_maxs = Vector2(newPlayerShipTextCoords.m_mins.y, newPlayerShipTextCoords.m_mins.x);

    AABB2D newPlayerShipSmallThrustTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(1, 5);
    m_playerShipSmallThrustTextureCoords.m_mins = Vector2(newPlayerShipSmallThrustTextCoords.m_maxs.y, newPlayerShipSmallThrustTextCoords.m_maxs.x);
    m_playerShipSmallThrustTextureCoords.m_maxs = Vector2(newPlayerShipSmallThrustTextCoords.m_mins.y, newPlayerShipSmallThrustTextCoords.m_mins.x);

    AABB2D newPlayerShipFullThrustTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(0, 5);
    m_playerShipFullThrustTextureCoords.m_mins = Vector2(newPlayerShipFullThrustTextCoords.m_maxs.y, newPlayerShipFullThrustTextCoords.m_maxs.x);
    m_playerShipFullThrustTextureCoords.m_maxs = Vector2(newPlayerShipFullThrustTextCoords.m_mins.y, newPlayerShipFullThrustTextCoords.m_mins.x);

    AABB2D newPlayerShipTurnRightTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(5, 6);
    m_playerShipTurnRightTextureCoords.m_mins = Vector2(newPlayerShipTurnRightTextCoords.m_maxs.y, newPlayerShipTurnRightTextCoords.m_maxs.x);
    m_playerShipTurnRightTextureCoords.m_maxs = Vector2(newPlayerShipTurnRightTextCoords.m_mins.y, newPlayerShipTurnRightTextCoords.m_mins.x);

    AABB2D newPlayerShipTurnRightSmallThrustTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(4, 6);
    m_playerShipTurnRightSmallThrustTextureCoords.m_mins = Vector2(newPlayerShipTurnRightSmallThrustTextCoords.m_maxs.y, newPlayerShipTurnRightSmallThrustTextCoords.m_maxs.x);
    m_playerShipTurnRightSmallThrustTextureCoords.m_maxs = Vector2(newPlayerShipTurnRightSmallThrustTextCoords.m_mins.y, newPlayerShipTurnRightSmallThrustTextCoords.m_mins.x);

    AABB2D newPlayerShipTurnRightFullThrustTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(3, 6);
    m_playerShipTurnRightFullThrustTextureCoords.m_mins = Vector2(newPlayerShipTurnRightFullThrustTextCoords.m_maxs.y, newPlayerShipTurnRightFullThrustTextCoords.m_maxs.x);
    m_playerShipTurnRightFullThrustTextureCoords.m_maxs = Vector2(newPlayerShipTurnRightFullThrustTextCoords.m_mins.y, newPlayerShipTurnRightFullThrustTextCoords.m_mins.x);

    AABB2D newPlayerShipTurnLeftTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(2, 6);
    m_playerShipTurnLeftTextureCoords.m_mins = Vector2(newPlayerShipTurnLeftTextCoords.m_maxs.y, newPlayerShipTurnLeftTextCoords.m_maxs.x);
    m_playerShipTurnLeftTextureCoords.m_maxs = Vector2(newPlayerShipTurnLeftTextCoords.m_mins.y, newPlayerShipTurnLeftTextCoords.m_mins.x);

    AABB2D newPlayerShipTurnLeftSmallThrustTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(1, 6);
    m_playerShipTurnLeftSmallThrustTextureCoords.m_mins = Vector2(newPlayerShipTurnLeftSmallThrustTextCoords.m_maxs.y, newPlayerShipTurnLeftSmallThrustTextCoords.m_maxs.x);
    m_playerShipTurnLeftSmallThrustTextureCoords.m_maxs = Vector2(newPlayerShipTurnLeftSmallThrustTextCoords.m_mins.y, newPlayerShipTurnLeftSmallThrustTextCoords.m_mins.x);

    AABB2D newPlayerShipTurnLeftFullThrustTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(0, 6);
    m_playerShipTurnLeftFullThrustTextureCoords.m_mins = Vector2(newPlayerShipTurnLeftFullThrustTextCoords.m_maxs.y, newPlayerShipTurnLeftFullThrustTextCoords.m_maxs.x);
    m_playerShipTurnLeftFullThrustTextureCoords.m_maxs = Vector2(newPlayerShipTurnLeftFullThrustTextCoords.m_mins.y, newPlayerShipTurnLeftFullThrustTextCoords.m_mins.x);
    //*******************************************************************

    AABB2D newPlayerBulletTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(0, 8);
    m_playerBulletTextureCoords.m_mins = Vector2(newPlayerBulletTextCoords.m_maxs.y, newPlayerBulletTextCoords.m_maxs.x);
    m_playerBulletTextureCoords.m_maxs = Vector2(newPlayerBulletTextCoords.m_mins.y, newPlayerBulletTextCoords.m_mins.x);

    AABB2D newStarburstTextCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(0, 8);
    m_starbustTextureCoords.m_mins = Vector2(newStarburstTextCoords.m_maxs.y, newStarburstTextCoords.m_maxs.x);
    m_starbustTextureCoords.m_maxs = Vector2(newStarburstTextCoords.m_mins.y, newStarburstTextCoords.m_mins.x);

    AABB2D newPickupTextureCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(9, 0);
    m_pickupTextureCoords.m_mins = Vector2(newPickupTextureCoords.m_maxs.y, newPickupTextureCoords.m_maxs.x);
    m_pickupTextureCoords.m_maxs = Vector2(newPickupTextureCoords.m_mins.y, newPickupTextureCoords.m_mins.x);

    AABB2D newMissileTextureCoords = m_spaceShipsSpriteSheet->GetTexCoordsForSpriteCoords(3, 8);
    m_enemyMissileTextureCoords.m_mins = Vector2(newMissileTextureCoords.m_maxs.y, newMissileTextureCoords.m_maxs.x);
    m_enemyMissileTextureCoords.m_maxs = Vector2(newMissileTextureCoords.m_mins.y, newMissileTextureCoords.m_mins.x);

    AABB2D newAsteroidTextCoords = m_asteroidsSpriteSheet->GetTexCoordsForSpriteCoords(0, 3);
    m_asteroidTextureCoords.m_mins = Vector2(newAsteroidTextCoords.m_maxs.y, newAsteroidTextCoords.m_maxs.x);
    m_asteroidTextureCoords.m_maxs = Vector2(newAsteroidTextCoords.m_mins.y, newAsteroidTextCoords.m_mins.x);

    AABB2D newAlienTextCoords = m_alienSpriteSheet->GetTexCoordsForSpriteCoords(0, 1);
    m_alienTextureCoords.m_mins = Vector2(newAlienTextCoords.m_maxs.y, newAlienTextCoords.m_maxs.x);
    m_alienTextureCoords.m_maxs = Vector2(newAlienTextCoords.m_mins.y, newAlienTextCoords.m_mins.x);

    AABB2D newAlienThrustingTextCoords = m_alienSpriteSheet->GetTexCoordsForSpriteCoords(0, 0);
    m_alienThrustingTextureCoords.m_mins = Vector2(newAlienThrustingTextCoords.m_maxs.y, newAlienThrustingTextCoords.m_maxs.x);
    m_alienThrustingTextureCoords.m_maxs = Vector2(newAlienThrustingTextCoords.m_mins.y, newAlienThrustingTextCoords.m_mins.x);

    std::string explosionSpriteSheetName = "Data/Images/Explosion.png";
    Texture2D* explosionSpriteSheetTexture = new Texture2D(g_theSimpleRenderer->m_device, explosionSpriteSheetName.c_str());
    m_explosionSpriteSheet = new SpriteSheet(explosionSpriteSheetTexture, 1, 7);

    std::string starfieldBackgroundName = "Data/Images/Starfield_Background.png";
    m_starfieldBackground = new Texture2D(g_theSimpleRenderer->m_device, starfieldBackgroundName.c_str());

    std::string emptyStarfieldBackgroundName = "Data/Images/EmptyStarfield_Background.png";
    m_emptyStarfieldBackground = new Texture2D(g_theSimpleRenderer->m_device, emptyStarfieldBackgroundName.c_str());

    m_bitmapFont = g_theSimpleRenderer->CreateOrGetFont("Data/Fonts/SquirrelFixedFont.png");

    //Spawn new gameplay elements that need to know about screen size
    SpawnStartingLives();
    SpawnStartingAsteroids();
    SpawnNewShip();
}

//*******************************************************************
void Game::Update(float deltaSeconds)
{
    PlayGameMusic(deltaSeconds);

    switch (m_currentGameState)
    {
    case START_GAME_STATE:  UpdateStartGameState(deltaSeconds); break;
    case GAME_STATE:        UpdateGameState(deltaSeconds);      break;
    case GAME_OVER_STATE:   UpdateGameOverState(deltaSeconds);  break;
    }
}

//*******************************************************************
// Credit to Squirrel Eiserloh
void Game::Render() const
{
    RenderSetup();

    switch (m_currentGameState)
    {
    case START_GAME_STATE:  RenderStartGameState(); break;
    case GAME_STATE:        RenderGameState();      break;
    case GAME_OVER_STATE:   RenderGameOverState();  break;
    }

}

//*******************************************************************
void Game::WrapAround()
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        Asteroid* asteroid = m_asteroids[asteroidIndex];
        if (asteroid != nullptr)
            asteroid->WrapAround();
    }

    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        Bullet* bullet = m_bullets[bulletIndex];
        if (bullet != nullptr)
            bullet->WrapAround();
    }

    if (m_ship != nullptr)
        m_ship->WrapAround();

    if (m_healthPickup != nullptr)
        m_healthPickup->WrapAround();

    if (m_starburst != nullptr)
        m_starburst->WrapAround();
}

//*******************************************************************
void Game::OnKeyDown(char keyCode)
{
    if (keyCode == KEY_ESCAPE)
        g_theApp->m_isQuitting = true;

    if ((m_ship == nullptr) && (keyCode == 'N'))
        SpawnNewShip();

    if (keyCode == 'P')
        m_IsPaused = !m_IsPaused;
}

//*******************************************************************
void Game::OnKeyUp(char keyCode)
{
    if (keyCode == 'I')
        SpawnNewAsteroid();

    if (keyCode == 'O')
        DestroyFirstAsteroid();

    if ((keyCode == 'B') && (m_playerHasStarBurst == true))
        UseStarburst();

    if (keyCode == KEY_SPACEBAR)
        SpawnNewBullet();
}

//*******************************************************************
bool Game::IsQuitting()
{
    return m_IsQuitting;
}

//*******************************************************************
void Game::RenderPlayerLives() const
{
    for (int lifeIndex = 0; lifeIndex < m_numberOfLivesForPlayer; ++lifeIndex)
    {
        m_currentLives[lifeIndex]->RenderPlayerLife();
    }
}

//*******************************************************************
void Game::UpdateStartGameState(float deltaSeconds)
{
    SetDisplayStartGameText(deltaSeconds);
    
    if (g_inputSystem->WasKeyJustPressed(KEY_ESCAPE))
        g_theApp->m_isQuitting = true;

    if (m_xboxController.WasButtonPressed(BUTTON_BACK))
        g_theApp->m_isQuitting = true;

    if (m_xboxController.WasButtonPressed(BUTTON_START))
    {
        PlaySound("Data/Audio/StartGame.wav");
        m_currentGameState = GAME_STATE;
    }

    if (g_inputSystem->WasKeyJustPressed(KEY_ENTER))
    {
        PlaySound("Data/Audio/StartGame.wav");
        m_currentGameState = GAME_STATE;
    }
}

//*******************************************************************
void Game::UpdateGameState(float deltaSeconds)
{
    SetScreenShake(deltaSeconds);
    SetDisplayStarburstText(deltaSeconds);
    SetDisplayNewLifeText(deltaSeconds);

    m_xboxController.Update();

    if (m_xboxController.WasButtonPressed(BUTTON_X))
        m_IsPaused = !m_IsPaused;

    if (g_inputSystem->WasKeyJustPressed('I'))
        SpawnNewAsteroid();

    if (g_inputSystem->WasKeyJustPressed('O'))
        DestroyFirstAsteroid();

    if (g_inputSystem->WasKeyJustPressed('S') && (m_playerHasStarBurst == true))
    {
        UseStarburst();
        PlaySound("Data/Audio/Starburst.mp3");
    }

    if (g_inputSystem->WasKeyJustPressed(KEY_SPACEBAR) || g_inputSystem->WasKeyJustPressed(MOUSE_LEFTBTTN))
        SpawnNewBullet();

    if (g_inputSystem->WasKeyJustPressed(KEY_ESCAPE))
        g_theApp->m_isQuitting = true;

    if (m_xboxController.WasButtonPressed(BUTTON_BACK))
        g_theApp->m_isQuitting = true;

    if ((m_ship == nullptr) && g_inputSystem->WasKeyJustPressed('N'))
        SpawnNewShip();

    if ((m_ship == nullptr) && m_xboxController.WasButtonPressed(BUTTON_START))
        SpawnNewShip();

    if (g_inputSystem->WasKeyJustPressed('P'))
        m_IsPaused = !m_IsPaused;

    if (m_xboxController.WasButtonPressed(BUTTON_BOTTOM_RIGHT))
        m_IsPaused = !m_IsPaused;

    deltaSeconds = deltaSeconds;

    if (m_IsPaused == true)
    {
        deltaSeconds = 0.f;
        return;
    }

    if (g_inputSystem->WasKeyJustPressed('T') == true)
        deltaSeconds *= 0.1f;

    if (m_xboxController.IsButtonPressed(BUTTON_B))
        deltaSeconds *= 0.1f;

    UpdateAsteroids(deltaSeconds);
    UpdateBullets(deltaSeconds);
    UpdateMissiles(deltaSeconds);
    UpdateExplosions(deltaSeconds);
    UpdateAlien(deltaSeconds);

    if (m_ship == nullptr)
        return;

    m_ship->m_acceleration.SetXY(0.f, 0.f);
    bool upKeyDown = g_inputSystem->IsKeyDown('W') || g_inputSystem->IsKeyDown(KEY_UPARROW);
    bool rightKeyDown = g_inputSystem->IsKeyDown('D') || g_inputSystem->IsKeyDown(KEY_RIGHTARROW);
    bool leftKeyDown = g_inputSystem->IsKeyDown('A') || g_inputSystem->IsKeyDown(KEY_LEFTARROW);

    if (upKeyDown && m_IsPaused == false)
        GetDirectionForShip('W');

    if (rightKeyDown)
        GetDirectionForShip('D');
    if (leftKeyDown)
        GetDirectionForShip('A');

    if (!rightKeyDown)
        DirectionNoLongerBeingUsed('D');
    if (!leftKeyDown)
        DirectionNoLongerBeingUsed('A');

    if (m_xboxController.WasButtonPressed(BUTTON_A))
        SpawnNewBullet();

    if (m_xboxController.WasButtonPressed(BUTTON_Y))
        UseStarburst();

    if (m_xboxController.m_leftJoyStick.m_magnitude > 0.f)
    {
        SetShipOrientationOffXboxLeftJoyStick(m_xboxController.m_leftJoyStick.m_angleDegrees);
        SetShipThrustOffXboxLeftJoyStick(m_xboxController.m_leftJoyStick.m_magnitude, m_xboxController.m_leftJoyStick.m_angleDegrees);
    }

    m_ship->Update(deltaSeconds);

    if (m_healthPickup != nullptr)
        m_healthPickup->Update(deltaSeconds);

    if (m_starburst != nullptr)
        m_starburst->Update(deltaSeconds);

    AsteroidAndShipCollide();
    AsteroidAndBulletCollide();
    SpawnMissileWhileAlienExists(deltaSeconds);


    if (m_alien != nullptr)
        AlienAndBulletCollide();
    if ((m_alien != nullptr) && (m_ship != nullptr))
        AlienAndShipCollide();
    if ((m_healthPickup != nullptr) && (m_ship != nullptr))
        ShipAndHealthCollide();
    if ((m_starburst != nullptr) && (m_ship != nullptr))
        ShipAndStarburstCollide();
    if (m_ship != nullptr)
        ShipAndMissileCollide();

    BounceAsteroids();

    CreateNewWaveOfAsteroids();
}

//*******************************************************************
void Game::UpdateGameOverState(float deltaSeconds)
{
    SetDisplayRestartGameText(deltaSeconds);

    if (g_inputSystem->WasKeyJustPressed(KEY_ESCAPE))
        g_theApp->m_isQuitting = true;

    if (g_inputSystem->WasKeyJustPressed(KEY_ENTER))
    {
        PlaySound("Data/Audio/StartGame.wav");
        ResetGame();
        m_currentGameState = GAME_STATE;
    }
}

//*******************************************************************
void Game::RenderStartGameState() const
{
    g_theSimpleRenderer->SetTexture(m_emptyStarfieldBackground);
    g_theSimpleRenderer->DrawQuad(Vector3(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f, 0.0f), Vector3(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f, 0.0f), Rgba::WHITE);
    g_theSimpleRenderer->SetTexture(g_theSimpleRenderer->m_whiteSample);

    g_theSimpleRenderer->DrawText2D(Vector2(300.0f, 375.0f), "A* 'ROIDS", 75.0f, Rgba::YELLOW, m_bitmapFont);

    if (m_displayStartGameText)
    {
        g_theSimpleRenderer->DrawText2D(Vector2(375.0f, 200.0f), "Press 'ENTER' to PLAY", 25.0f, Rgba::YELLOW, m_bitmapFont);
    }
}

//*******************************************************************
void Game::RenderGameState() const
{
    if (m_screenShake != Vector2())
    {
        Matrix4 newViewMatrix = g_theSimpleRenderer->matrix_data.view;
        newViewMatrix.Translate(Vector3(m_screenShake.x, m_screenShake.y, 0.f));
        g_theSimpleRenderer->SetViewMatrix(newViewMatrix);
    }

    g_theSimpleRenderer->SetTexture(m_starfieldBackground);
    g_theSimpleRenderer->DrawQuad(Vector3(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f, 0.0f), Vector3(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f, 0.0f), Rgba::WHITE);
    g_theSimpleRenderer->SetTexture(g_theSimpleRenderer->m_whiteSample);

    // Draw the asteroids
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        Asteroid* asteroid = m_asteroids[asteroidIndex];
        if (asteroid != nullptr)
            asteroid->Render();
    }

    // Draw the ship
    if (m_ship != nullptr)
        m_ship->Render();

    if (m_healthPickup != nullptr)
        m_healthPickup->RenderHealthPickup();

    if (m_starburst != nullptr)
        m_starburst->Render();

    if (m_alien != nullptr)
        m_alien->Render();

    // Draw the bullets
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        Bullet* bullet = m_bullets[bulletIndex];
        if (bullet != nullptr)
            bullet->Render();
    }

    for (int missileIndex = 0; missileIndex < MAX_BULLETS; ++missileIndex)
    {
        Missile* missile = m_missiles[missileIndex];
        if (missile != nullptr)
            missile->Render();
    }

    RenderExplosions();
    RenderPlayerLives();

    if (m_playerHasStarBurst && m_displayStarburstText && (m_ship != nullptr))
    {
        g_theSimpleRenderer->DrawText2D(Vector2(250.0f, 25.0f), "Press 'S' to ACTIVATE STARBURST", 25.0f, Rgba::YELLOW, m_bitmapFont);
    }

    if ((m_ship == nullptr) && m_displayNewLifeText)
    {
        g_theSimpleRenderer->DrawText2D(Vector2(275.0f, 325.0f), "Press 'N' to COMEBACK TO LIFE", 25.0f, Rgba::YELLOW, m_bitmapFont);
    }

    std::string scoreString = Stringf("SCORE: %i", m_score);
    g_theSimpleRenderer->DrawText2D(Vector2(500.0f, 690.0f), scoreString, 25.0f, Rgba::YELLOW, m_bitmapFont);
}

//*******************************************************************
void Game::RenderGameOverState() const
{
    g_theSimpleRenderer->SetTexture(m_emptyStarfieldBackground);
    g_theSimpleRenderer->DrawQuad(Vector3(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f, 0.0f), Vector3(ORHTO_WIDTH / 2.0f, ORHTO_HEIGHT / 2.0f, 0.0f), Rgba::WHITE);
    g_theSimpleRenderer->SetTexture(g_theSimpleRenderer->m_whiteSample);

    g_theSimpleRenderer->DrawText2D(Vector2(300.0f, 375.0f), "GAME OVER", 75.0f, Rgba::RED, m_bitmapFont);

    std::string finalString = Stringf("FINAL SCORE: %i", m_score);
    g_theSimpleRenderer->DrawText2D(Vector2(425.0f, 300.0f), finalString, 25.0f, Rgba::RED, m_bitmapFont);

    if (m_displayRestartGameText)
    {
        g_theSimpleRenderer->DrawText2D(Vector2(300.0f, 200.0f), "Press 'ENTER' to PLAY AGAIN", 25.0f, Rgba::RED, m_bitmapFont);
    }
}

//*******************************************************************
void Game::RenderSetup() const
{
    g_theSimpleRenderer->matrix_data.view = Matrix4();

    g_theSimpleRenderer->SetRenderTarget(nullptr);
    g_theSimpleRenderer->ClearColor(Rgba(0, 0, 0, 255));
    g_theSimpleRenderer->ClearDepth();
    g_theSimpleRenderer->EnableDepthTest(false);
    g_theSimpleRenderer->SetViewport(0, 0, (unsigned int)g_theSimpleRenderer->m_output->GetWidth(), (unsigned int)g_theSimpleRenderer->m_output->GetHeight());

    g_theSimpleRenderer->SetSampler(g_theSimpleRenderer->m_pointSampler);
    g_theSimpleRenderer->SetTexture(g_theSimpleRenderer->m_whiteSample);
    g_theSimpleRenderer->SetOrthoProjection(Vector2(0.0f, 0.0f), Vector2(ORHTO_WIDTH, ORHTO_HEIGHT));
    g_theSimpleRenderer->SetShader(g_theSimpleRenderer->m_unlitShader);
}

//*******************************************************************
void Game::RenderExplosions() const
{
    for (int explosionIndex = 0; explosionIndex < (int)m_explosions.size(); ++explosionIndex)
    {
        Explosion* explosion = m_explosions[explosionIndex];
        if (explosion != nullptr)
            explosion->RenderDX();
    }
}

//*******************************************************************
void Game::UpdateAsteroids(float deltaSeconds)
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        Asteroid* asteroid = m_asteroids[asteroidIndex];
        if (asteroid != nullptr)
            asteroid->Update(deltaSeconds);
    }
}

//*******************************************************************
void Game::SpawnNewAsteroid()
{
    Asteroid* newAsteroid = new Asteroid();
    int positiveOrNegative = GetRandomIntLessThan(2);
    if (positiveOrNegative == 0) {
        newAsteroid->m_position.x = GetRandomFloatInRange(0.f, ORHTO_WIDTH);
        newAsteroid->m_position.y = GetRandomFloatInRange(-100.f, 0.f - (newAsteroid->m_liberalRadius.radius*3.5f));
    }
    else {
        newAsteroid->m_position.x = GetRandomFloatInRange(-100.f, 0.f - (newAsteroid->m_liberalRadius.radius*3.5f));
        newAsteroid->m_position.y = GetRandomFloatInRange(0.f, ORHTO_HEIGHT);
    }
    newAsteroid->m_velocity.x = GetRandomFloatInRange(-100.f, 100.f);
    newAsteroid->m_velocity.y = GetRandomFloatInRange(-100.f, 100.f);
    newAsteroid->m_spinDegreesPerSecond = GetRandomFloatInRange(-500.f, 500.f);
    newAsteroid->m_orientationDegrees = GetRandomFloatInRange(0.f, 360.f);
    AddToAsteroidsList(newAsteroid);
}

//*******************************************************************
void Game::SpawnNewBossAsteroid()
{
    Asteroid* newBossAsteroid = new Asteroid();
    newBossAsteroid->m_radius = 100.f;
    newBossAsteroid->SetRadiiForPhysicalAndLiberalRadii();
    int positiveOrNegative = GetRandomIntLessThan(2);
    if (positiveOrNegative == 0) {
        newBossAsteroid->m_position.x = GetRandomFloatInRange(0.f, ORHTO_WIDTH);
        newBossAsteroid->m_position.y = GetRandomFloatInRange(-100.f, 0.f - (newBossAsteroid->m_liberalRadius.radius*3.5f));
    }
    else {
        newBossAsteroid->m_position.x = GetRandomFloatInRange(-100.f, 0.f - (newBossAsteroid->m_liberalRadius.radius*3.5f));
        newBossAsteroid->m_position.y = GetRandomFloatInRange(0.f, ORHTO_HEIGHT);
    }
    newBossAsteroid->m_velocity.x = GetRandomFloatInRange(-50.f, 50.f);
    newBossAsteroid->m_velocity.y = GetRandomFloatInRange(-50.f, 50.f);
    newBossAsteroid->m_spinDegreesPerSecond = GetRandomFloatInRange(-250.f, 250.f);
    newBossAsteroid->m_orientationDegrees = GetRandomFloatInRange(0.f, 360.f);
    AddToAsteroidsList(newBossAsteroid);
}

//*******************************************************************
void Game::DuplicateAsteroid(Asteroid* asteroid)
{
    Asteroid* newAsteroid = new Asteroid(*(asteroid));
    newAsteroid->m_velocity.x = GetRandomFloatInRange(-100.f, 100.f);
    newAsteroid->m_velocity.y = GetRandomFloatInRange(-100.f, 100.f);
    AddToAsteroidsList(newAsteroid);
}

//*******************************************************************
void Game::DestroyFirstAsteroid()
{
    DestroyAsteroid(0);
}

//*******************************************************************
void Game::DestroyAsteroid(int asteroidIndex)
{
    delete m_asteroids[asteroidIndex];
    for (; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex) {
        if (asteroidIndex == (MAX_ASTEROIDS - 1))
        {
            m_asteroids[asteroidIndex] = nullptr;
            break;
        }
        else if (m_asteroids[asteroidIndex] == nullptr)
        {
            break;
        }
        else
        {
            m_asteroids[asteroidIndex] = nullptr;
            m_asteroids[asteroidIndex] = m_asteroids[asteroidIndex + 1];
        }
    }
}

//*******************************************************************
void Game::BreakAsteroid(Asteroid* asteroid, int asteroidIndex)
{
    float asteroid_radius = asteroid->m_radius;
    CalculateNewScore(asteroid_radius);
    if (asteroid_radius <= MIN_RADIUS_OF_ASTEROID)
    {
        DestroyAsteroid(asteroidIndex);
        return;
    }
    asteroid->m_radius = asteroid_radius*0.75f;
    asteroid->SetRadiiForPhysicalAndLiberalRadii();
    DuplicateAsteroid(asteroid);
}

//*******************************************************************
// Credit to Squirrel Eiserloh
void Game::AddToAsteroidsList(Asteroid* newAsteroid)
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        if (m_asteroids[asteroidIndex] == nullptr)
        {
            m_asteroids[asteroidIndex] = newAsteroid;
            return;
        }
    }
}

//*******************************************************************
void Game::DestroyAllAsteroids()
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        if (m_asteroids[asteroidIndex] != nullptr)
        {
            delete m_asteroids[asteroidIndex];
            m_asteroids[asteroidIndex] = nullptr;
        }
    }
}

//*******************************************************************
void Game::CreateNewWaveOfAsteroids()
{
    if (m_asteroids[0] != nullptr)
        return;

    CURRENT_WAVE++;
    if (CURRENT_WAVE % BOSS_ASTEROID_MOD == 0)
    {
        for (int bossAsteroidIndex = 0; bossAsteroidIndex < NUM_BOSS_ASTEROIDS; ++bossAsteroidIndex)
        {
            SpawnNewBossAsteroid();
        }
        NUM_BOSS_ASTEROIDS += 3;
        return;
    }

    if ((CURRENT_WAVE % NEW_ALIEN_MOD == 0) && (m_alien == nullptr))
        SpawnNewAlien();

    if ((CURRENT_WAVE % NEW_LIFE_MOD == 0) && (m_healthPickup == nullptr))
        SpawnNewLife();

    if ((CURRENT_WAVE % NEW_STARBURST_MOD == 0) && (m_starburst == nullptr) && (m_playerHasStarBurst == false))
        SpawnNewStarburst();

    NUM_STARTING_ASTEROIDS += 3;
    for (int asteroidIndex = 0; asteroidIndex < NUM_STARTING_ASTEROIDS; ++asteroidIndex)
    {
        SpawnNewAsteroid();
    }
}

//*******************************************************************
void Game::BounceAsteroids()
{
    for (int asteroidIndex = 0; asteroidIndex < (MAX_ASTEROIDS - 1); ++asteroidIndex)
    {
        for (int followingAsteroidIndex = asteroidIndex + 1; followingAsteroidIndex < MAX_ASTEROIDS; ++followingAsteroidIndex)
        {
            if ((m_asteroids[asteroidIndex] != nullptr) && (m_asteroids[followingAsteroidIndex] != nullptr))
            {
                BounceDiscs(m_asteroids[asteroidIndex]->m_liberalRadius, m_asteroids[followingAsteroidIndex]->m_liberalRadius, m_asteroids[asteroidIndex]->m_velocity, m_asteroids[followingAsteroidIndex]->m_velocity);
                m_asteroids[asteroidIndex]->m_position = m_asteroids[asteroidIndex]->m_liberalRadius.center;
                m_asteroids[followingAsteroidIndex]->m_position = m_asteroids[followingAsteroidIndex]->m_liberalRadius.center;
                m_asteroids[asteroidIndex]->SetCenterForPhysicalAndLiberalRadii();
                m_asteroids[followingAsteroidIndex]->SetCenterForPhysicalAndLiberalRadii();
            }
        }
    }
}

//*******************************************************************
void Game::SpawnStartingAsteroids()
{
    for (int i = 0; i < NUM_STARTING_ASTEROIDS; ++i)
    {
        SpawnNewAsteroid();
    }
}

//*******************************************************************
void Game::SpawnNewAlien()
{
    m_alien = new Alien();
    m_alien->m_position.x = 0.f - (m_alien->m_radius * 2.0f);
    m_alien->m_position.y = GetRandomFloatInRange(ORHTO_HEIGHT - (m_alien->m_radius * 2.0f), m_alien->m_radius * 2.0f);
    m_alien->m_velocity.x = 100.0f;
}

//*******************************************************************
void Game::DestroyAlien()
{
    if (m_alien != nullptr)
    {
        delete m_alien;
        m_alien = nullptr;
        m_currentTimeAlienExists = 0.0f;
        m_timeSinceLastMissileWasSpawned = 0.0f;
    }
}

//*******************************************************************
void Game::UpdateAlien(float deltaSeconds)
{
    if (m_alien != nullptr)
    {
        if (m_currentTimeAlienExists < TIME_ALIEN_IS_ON_SCREEN)
        {
            m_currentTimeAlienExists += deltaSeconds;
            m_alien->Update(deltaSeconds);
        }
        else
        {
            DestroyAlien();
        }

    }
}

//*******************************************************************
void Game::SpawnStartingLives()
{
    for (int lifeIndex = 0; lifeIndex < NUM_LIVES_FOR_PLAYER; ++lifeIndex)
    {
        m_currentLives[lifeIndex] = new Ship();
        m_currentLives[lifeIndex]->m_radius /= 1.5f;
        m_currentLives[lifeIndex]->m_position = PLAYER_LIVES_POSITION + ((float)lifeIndex * Vector2(m_currentLives[lifeIndex]->m_radius * 2.5f, 0.f));
    }
}

//*******************************************************************
void Game::SpawnNewShip() {
    if (m_numberOfLivesForPlayer > 0)
        m_ship = new Ship();
}

//*******************************************************************
void Game::DestroyShip()
{
    SetRandomScreenShake();

    if (m_ship != nullptr)
    {
        delete m_ship;
        m_ship = nullptr;
    }
    RemoveLife();
    m_numberOfLivesForPlayer--;
    if (m_numberOfLivesForPlayer == 0)
    {
        m_currentGameState = GAME_OVER_STATE;
        //m_IsQuitting = true;
    }
}

//*******************************************************************
void Game::GetDirectionForShip(char direction)
{
    if (direction == 'W')
    {
        m_ship->m_directionUp = true;
        m_ship->CalculateThrustKeyboard();
    }
    if (direction == 'D')
        m_ship->m_directionRight = true;
    if (direction == 'A')
        m_ship->m_directionLeft = true;
}

//*******************************************************************
void Game::DirectionNoLongerBeingUsed(char direction)
{
    if (direction == 'W')
        m_ship->m_directionUp = false;
    if (direction == 'D')
        m_ship->m_directionRight = false;
    if (direction == 'A')
        m_ship->m_directionLeft = false;
}

//*******************************************************************
void Game::SetShipOrientationOffXboxLeftJoyStick(float leftJoyStickAngleDegrees)
{
    m_ship->m_orientationDegrees = leftJoyStickAngleDegrees;
}

//*******************************************************************
void Game::SetShipThrustOffXboxLeftJoyStick(float leftJoyStickR, float leftJoyStickAngle)
{
    m_ship->CalculateThrustXboxController(leftJoyStickR, leftJoyStickAngle);
}

//*******************************************************************
void Game::RemoveLife()
{
    delete m_currentLives[m_numberOfLivesForPlayer - 1];
    m_currentLives[m_numberOfLivesForPlayer - 1] = nullptr;
}

//*******************************************************************
void Game::RemoveAllLives()
{
    for (int lifeIndex = 0; lifeIndex < m_numberOfLivesForPlayer; ++lifeIndex)
    {
        if (m_currentLives[lifeIndex] != nullptr)
        {
            delete m_currentLives[lifeIndex];
            m_currentLives[lifeIndex] = nullptr;
        }
    }
}

//*******************************************************************
void Game::SpawnNewLife()
{
    Ship* newLife = new Ship();
    newLife->m_radius /= 1.5f;

    int positiveOrNegative = GetRandomIntLessThan(2);
    if (positiveOrNegative == 0) {
        newLife->m_position.x = GetRandomFloatInRange(0.f, ORHTO_WIDTH);
        newLife->m_position.y = GetRandomFloatInRange(-100.f, 0.f - (newLife->m_liberalRadius.radius*3.5f));
    }
    else {
        newLife->m_position.x = GetRandomFloatInRange(-100.f, 0.f - (newLife->m_liberalRadius.radius*3.5f));
        newLife->m_position.y = GetRandomFloatInRange(0.f, ORHTO_HEIGHT);
    }
    newLife->m_velocity.x = GetRandomFloatInRange(-100.f, 100.f);
    newLife->m_velocity.y = GetRandomFloatInRange(-100.f, 100.f);
    newLife->m_spinDegreesPerSecond = GetRandomFloatInRange(-500.f, 500.f);
    newLife->m_orientationDegrees = GetRandomFloatInRange(0.f, 360.f);
    m_healthPickup = newLife;
}

//*******************************************************************
void Game::DestroyHealthPickup()
{
    if (m_healthPickup != nullptr)
    {
        delete m_healthPickup;
        m_healthPickup = nullptr;
    }
}

//*******************************************************************
void Game::AddLifeToLivesList()
{
    for (int lifeIndex = 0; lifeIndex < NUM_LIVES_FOR_PLAYER; ++lifeIndex)
    {
        if (m_currentLives[lifeIndex] == nullptr)
        {
            m_currentLives[lifeIndex] = new Ship();
            m_currentLives[lifeIndex]->m_radius /= 1.5f;
            m_currentLives[lifeIndex]->m_position = PLAYER_LIVES_POSITION + ((float)lifeIndex * Vector2(m_currentLives[lifeIndex]->m_radius * 2.5f, 0.f));
            m_numberOfLivesForPlayer++;
            PlaySound("Data/Audio/PickupStarburst.wav");
            break;
        }
    }
    DestroyHealthPickup();
}

//*******************************************************************
void Game::SpawnNewStarburst()
{
    Starburst* newStarburst = new Starburst();
    int positiveOrNegative = GetRandomIntLessThan(2);
    if (positiveOrNegative == 0) {
        newStarburst->m_position.x = GetRandomFloatInRange(0.f, ORHTO_WIDTH);
        newStarburst->m_position.y = GetRandomFloatInRange(-100.f, 0.f - (newStarburst->m_liberalRadius.radius*3.5f));
    }
    else {
        newStarburst->m_position.x = GetRandomFloatInRange(-100.f, 0.f - (newStarburst->m_liberalRadius.radius*3.5f));
        newStarburst->m_position.y = GetRandomFloatInRange(0.f, ORHTO_HEIGHT);
    }
    newStarburst->m_velocity.x = GetRandomFloatInRange(-100.f, 100.f);
    newStarburst->m_velocity.y = GetRandomFloatInRange(-100.f, 100.f);
    newStarburst->m_spinDegreesPerSecond = GetRandomFloatInRange(-250.f, 250.f);
    newStarburst->m_orientationDegrees = GetRandomFloatInRange(0.f, 360.f);
    m_starburst = newStarburst;
}

//*******************************************************************
void Game::DestroyStarburst()
{
    if (m_starburst != nullptr)
    {
        delete m_starburst;
        m_starburst = nullptr;
    }
}

//*******************************************************************
void Game::UseStarburst()
{
    DestroyAllBullets();
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        m_bullets[bulletIndex] = new Bullet();
        m_bullets[bulletIndex]->m_position.x = m_ship->m_position.x;
        m_bullets[bulletIndex]->m_position.y = m_ship->m_position.y;
        m_bullets[bulletIndex]->m_orientationDegrees = MAX_BULLETS * (float)bulletIndex;
        m_bullets[bulletIndex]->m_velocity.x = 620.f*CosDegrees(m_bullets[bulletIndex]->m_orientationDegrees);
        m_bullets[bulletIndex]->m_velocity.y = 620.f*SinDegrees(m_bullets[bulletIndex]->m_orientationDegrees);
    }
    m_playerHasStarBurst = false;
}

//*******************************************************************
void Game::AddStarburstForPlayerUse()
{
    m_playerHasStarBurst = true;
    DestroyStarburst();
    PlaySound("Data/Audio/PickupStarburst.wav");
}

//*******************************************************************
void Game::AsteroidAndShipCollide()
{
    for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
    {
        if (m_asteroids[asteroidIndex] != nullptr)
        {
            bool asteroidAndShipCollide = DoDisc2DsOverlap(m_asteroids[asteroidIndex]->m_physicalRadius, m_ship->m_physicalRadius);
            if (asteroidAndShipCollide)
            {
                BreakAsteroid(m_asteroids[asteroidIndex], asteroidIndex);

                GetOrCreateExplosion(true, nullptr, nullptr);
                Vector2 shipPosition = m_ship->m_position;
                DestroyShip();
                DestroyExplosion(shipPosition);
                PlaySound("Data/Audio/Explosion.wav");
                return;
            }
        }
    }
}

//*******************************************************************
void Game::AsteroidAndBulletCollide()
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        for (int asteroidIndex = 0; asteroidIndex < MAX_ASTEROIDS; ++asteroidIndex)
        {
            if (m_bullets[bulletIndex] != nullptr)
            {
                if (m_asteroids[asteroidIndex] != nullptr)
                {
                    bool asteroidAndBulletCollide = DoDisc2DsOverlap(m_asteroids[asteroidIndex]->m_physicalRadius, m_bullets[bulletIndex]->m_physicalRadius);
                    if (asteroidAndBulletCollide)
                    {
                        BreakAsteroid(m_asteroids[asteroidIndex], asteroidIndex);

                        GetOrCreateExplosion(false, m_bullets[bulletIndex], nullptr);
                        Vector2 bulletPosition = m_bullets[bulletIndex]->m_position;
                        DestroyBullet(bulletIndex);
                        DestroyExplosion(bulletPosition);
                        PlaySound("Data/Audio/Explosion.wav");
                        return;
                    }
                }
            }
        }
    }
}

//*******************************************************************
void Game::ShipAndHealthCollide()
{
    bool shipAndHealthCollide = DoDisc2DsOverlap(m_ship->m_physicalRadius, m_healthPickup->m_physicalRadius);
    if (shipAndHealthCollide)
    {
        AddLifeToLivesList();
    }
}

//*******************************************************************
void Game::ShipAndStarburstCollide()
{
    bool shipAndStarburstCollide = DoDisc2DsOverlap(m_ship->m_physicalRadius, m_starburst->m_physicalRadius);
    if (shipAndStarburstCollide)
    {
        AddStarburstForPlayerUse();
    }
}

//*******************************************************************
void Game::AlienAndBulletCollide()
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        if (m_bullets[bulletIndex] != nullptr)
        {
            bool alienAndBulletCollide = DoDisc2DsOverlap(m_alien->m_physicalRadius, m_bullets[bulletIndex]->m_physicalRadius);
            if (alienAndBulletCollide)
            {
                GetOrCreateExplosion(false, m_bullets[bulletIndex], nullptr);
                Vector2 bulletPosition = m_bullets[bulletIndex]->m_position;
                DestroyBullet(bulletIndex);
                DestroyExplosion(bulletPosition);
                PlaySound("Data/Audio/Explosion.wav");

                GetOrCreateExplosion(false, nullptr, nullptr);
                Vector2 alienPosition = m_alien->m_position;
                DestroyAlien();
                DestroyExplosion(alienPosition);
                PlaySound("Data/Audio/Explosion.wav");

                m_score = m_score + 1000;
                return;
            }
        }
    }
}

//*******************************************************************
void Game::AlienAndShipCollide()
{
    bool alienAndShipCollide = DoDisc2DsOverlap(m_ship->m_physicalRadius, m_alien->m_physicalRadius);
    if (alienAndShipCollide)
    {
        GetOrCreateExplosion(true, nullptr, nullptr);
        Vector2 shipPosition = m_ship->m_position;
        DestroyShip();
        DestroyExplosion(shipPosition);
        PlaySound("Data/Audio/Explosion.wav");

        GetOrCreateExplosion(false, nullptr, nullptr);
        Vector2 alienPosition = m_alien->m_position;
        DestroyAlien();
        DestroyExplosion(alienPosition);
        PlaySound("Data/Audio/Explosion.wav");
    }
}

//*******************************************************************
void Game::ShipAndMissileCollide()
{
    for (int missileIndex = 0; missileIndex < MAX_BULLETS; ++missileIndex)
    {
        if (m_missiles[missileIndex] != nullptr)
        {
            bool shipAndMissileCollide = DoDisc2DsOverlap(m_ship->m_physicalRadius, m_missiles[missileIndex]->m_physicalRadius);
            if (shipAndMissileCollide)
            {
                GetOrCreateExplosion(false, nullptr, m_missiles[missileIndex]);
                Vector2 missilePosition = m_missiles[missileIndex]->m_position;
                DestroyMissile(missileIndex);
                DestroyExplosion(missilePosition);
                PlaySound("Data/Audio/Explosion.wav");

                GetOrCreateExplosion(true, nullptr, nullptr);
                Vector2 shipPosition = m_ship->m_position;
                DestroyShip();
                DestroyExplosion(shipPosition);
                PlaySound("Data/Audio/Explosion.wav");
                return;
            }
        }
    }
}

//*******************************************************************
void Game::CalculateNewScore(float asteroidRadius)
{
    int scoreMultiplier = 100 / (int)asteroidRadius;
    m_score = m_score + (50 * scoreMultiplier);
}

//*******************************************************************
void Game::UpdateBullets(float deltaSeconds)
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        Bullet* bullet = m_bullets[bulletIndex];
        if (bullet != nullptr)
        {
            bullet->Update(deltaSeconds);
            if (bullet->m_totalDeltaSeconds >= BULLET_LIFETIME_SECONDS)
                DestroyBullet(bulletIndex);
        }
    }
}

//*******************************************************************
void Game::SpawnNewBullet()
{
    if (m_ship == nullptr)
        return;
    Bullet* newBullet = new Bullet();
    newBullet->m_position.x = m_ship->m_position.x + m_ship->m_radius*CosDegrees(m_ship->m_orientationDegrees);
    newBullet->m_position.y = m_ship->m_position.y + m_ship->m_radius*SinDegrees(m_ship->m_orientationDegrees);
    newBullet->m_velocity.x = 620.f*CosDegrees(m_ship->m_orientationDegrees);
    newBullet->m_velocity.y = 620.f*SinDegrees(m_ship->m_orientationDegrees);
    newBullet->m_orientationDegrees = m_ship->m_orientationDegrees;
    AddToBulletsList(newBullet);

    PlaySound("Data/Audio/FireBullet.wav");
}

//*******************************************************************
void Game::AddToBulletsList(Bullet* newBullet)
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        if (m_bullets[bulletIndex] == nullptr)
        {
            m_bullets[bulletIndex] = newBullet;
            return;
        }
    }
}

//*******************************************************************
void Game::DestroyFirstBullet()
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex) {
        if (m_bullets[bulletIndex] != nullptr)
        {
            delete m_bullets[bulletIndex];
            m_bullets[bulletIndex] = nullptr;
            return;
        }
    }
}

//*******************************************************************
void Game::DestroyBullet(int bulletIndex)
{
    delete m_bullets[bulletIndex];
    for (; bulletIndex < MAX_BULLETS; ++bulletIndex) {
        if (bulletIndex == (MAX_BULLETS - 1))
        {
            m_bullets[bulletIndex] = nullptr;
            break;
        }
        else if (m_bullets[bulletIndex] == nullptr)
        {
            break;
        }
        else
        {
            m_bullets[bulletIndex] = nullptr;
            m_bullets[bulletIndex] = m_bullets[bulletIndex + 1];
        }
    }
}

//*******************************************************************
void Game::DestroyAllBullets()
{
    for (int bulletIndex = 0; bulletIndex < MAX_BULLETS; ++bulletIndex)
    {
        if (m_bullets[bulletIndex] != nullptr)
        {
            delete m_bullets[bulletIndex];
            m_bullets[bulletIndex] = nullptr;
        }
    }
}

//*******************************************************************
void Game::UpdateMissiles(float deltaSeconds)
{
    for (int missileIndex = 0; missileIndex < MAX_BULLETS; ++missileIndex)
    {
        Missile* missile = m_missiles[missileIndex];
        if (m_ship == nullptr)
        {
            DestroyMissile(missileIndex);
            continue;
        }

        if (missile != nullptr)
        {
            missile->Update(deltaSeconds);
            if (missile->m_totalDeltaSeconds >= MISSILE_LIFETIME_SECONDS)
                DestroyMissile(missileIndex);
        }
    }
}

//*******************************************************************
void Game::SpawnNewMissile()
{
    if (m_alien == nullptr)
        return;
    Missile* newMissile = new Missile();
    float randomOrientation = GetRandomFloatInRange(0.0f, 360.0f);
    newMissile->m_position.x = m_alien->m_position.x + m_alien->m_radius*CosDegrees(randomOrientation);
    newMissile->m_position.y = m_alien->m_position.y + m_alien->m_radius*SinDegrees(randomOrientation);
    newMissile->m_velocity.x = 620.f*CosDegrees(randomOrientation);
    newMissile->m_velocity.y = 620.f*SinDegrees(randomOrientation);
    newMissile->m_orientationDegrees = m_alien->m_orientationDegrees;
    AddToMissilesList(newMissile);

    PlaySound("Data/Audio/FireBullet.wav");
}

//*******************************************************************
void Game::AddToMissilesList(Missile* newMissile)
{
    for (int missileIndex = 0; missileIndex < MAX_BULLETS; ++missileIndex)
    {
        if (m_missiles[missileIndex] == nullptr)
        {
            m_missiles[missileIndex] = newMissile;
            return;
        }
    }
}

//*******************************************************************
void Game::DestroyMissile(int missileIndex)
{
    delete m_missiles[missileIndex];
    for (; missileIndex < MAX_BULLETS; ++missileIndex) {
        if (missileIndex == (MAX_BULLETS - 1))
        {
            m_missiles[missileIndex] = nullptr;
            break;
        }
        else if (m_missiles[missileIndex] == nullptr)
        {
            break;
        }
        else
        {
            m_missiles[missileIndex] = nullptr;
            m_missiles[missileIndex] = m_missiles[missileIndex + 1];
        }
    }
}

//*******************************************************************
void Game::DestroyAllMissiles()
{
    for (int missileIndex = 0; missileIndex < MAX_BULLETS; ++missileIndex)
    {
        if (m_missiles[missileIndex] != nullptr)
        {
            delete m_missiles[missileIndex];
            m_missiles[missileIndex] = nullptr;
        }
    }
}

//*******************************************************************
void Game::SpawnMissileWhileAlienExists(float deltaSeconds)
{
    if (m_alien != nullptr)
    {
        if (m_timeSinceLastMissileWasSpawned < TIME_TO_SPAWN_NEW_MISSILE)
        {
            m_timeSinceLastMissileWasSpawned += deltaSeconds;
            m_alien->Update(deltaSeconds);
        }
        else
        {
            SpawnNewMissile();
            m_timeSinceLastMissileWasSpawned = 0.0f;
        }
    }
}

//*******************************************************************
void Game::DestroyAllExplosions()
{
    for (int explosionIndex = 0; explosionIndex < (int)m_explosions.size(); ++explosionIndex)
    {
        if (m_explosions[explosionIndex] != nullptr)
        {
            delete m_explosions[explosionIndex];
            m_explosions[explosionIndex] = nullptr;
        }

    }
    if (m_explosions.size() > 0)
        m_explosions.clear();
}

//*******************************************************************
void Game::GetOrCreateExplosion(bool isShip, Bullet* bullet, Missile* missile)
{
    Explosion* existingExplosion = nullptr;
    for (int explosionIndex = 0; explosionIndex < (int)m_explosions.size(); explosionIndex++)
    {
        Explosion* explosionFound = m_explosions[explosionIndex];
        if (isShip)
        {
            if (explosionFound && explosionFound->m_position == m_ship->m_position)
            {
                existingExplosion = explosionFound;
            }
        }
        else
        {
            if (bullet != nullptr)
            {
                if (explosionFound && explosionFound->m_position == bullet->m_position)
                {
                    existingExplosion = explosionFound;
                }
            }
        }
    }

    if (existingExplosion == nullptr)
        GenerateExplosion(isShip, bullet, missile);
}

//*******************************************************************
void Game::GenerateExplosion(bool isShip, Bullet* bullet, Missile* missile)
{
    float duration = 1.f;
    SpriteAnimation animation(*m_explosionSpriteSheet, duration, SPRITE_ANIM_MODE_PLAY_TO_END, 0, 6);

    Explosion* explosion = nullptr;

    if (isShip)
    {
        explosion = new Explosion(animation, m_ship->m_position, m_ship->m_radius);
    }
    else
    {
        if (bullet != nullptr)
        {
            explosion = new Explosion(animation, bullet->m_position, bullet->m_radius);
        }
        else
        {
            if (missile != nullptr)
            {
                explosion = new Explosion(animation, missile->m_position, missile->m_radius);
            }
            else
            {
                explosion = new Explosion(animation, m_alien->m_position, m_alien->m_radius);
            }
        }
    }

    m_explosions.push_back(explosion);
}


//*******************************************************************
void Game::UpdateExplosions(float deltaSeconds)
{
    for (int explosionsIndex = 0; explosionsIndex < (int)m_explosions.size(); ++explosionsIndex)
    {
        Explosion* explosion = m_explosions[explosionsIndex];
        if (explosion != nullptr)
        {
            explosion->m_spriteAnimation.Resume();
            explosion->Update(deltaSeconds);
        }

    }
}

//*******************************************************************
void Game::DestroyExplosion(Vector2 entityPosition)
{
    for (int explosionIndex = 0; explosionIndex < (int)m_explosions.size(); explosionIndex++)
    {
        Explosion* explosion = m_explosions[explosionIndex];
        if (!explosion)
            continue;

        if (explosion->m_position == entityPosition && explosion->m_spriteAnimation.IsFinished() == true)
        {
            delete m_explosions[explosionIndex];
            m_explosions[explosionIndex] = nullptr;
        }
        if (explosion->m_position == entityPosition && explosion->m_spriteAnimation.IsFinished() == false)
        {
            return;
        }
    }
}

//*******************************************************************
void Game::ResetGame()
{
    m_playerHasStarBurst = false;
    m_numberOfLivesForPlayer = NUM_LIVES_FOR_PLAYER;
    NUM_STARTING_ASTEROIDS = 4;
    NUM_BOSS_ASTEROIDS = 2;
    CURRENT_WAVE = 1;
    m_score = 0;

    SetScreenShakeToZero();

    DestroyHealthPickup();
    DestroyStarburst();
    DestroyAlien();
    DestroyAllAsteroids();
    DestroyAllBullets();
    DestroyAllMissiles();
    DestroyAllExplosions();

    SpawnStartingLives();
    SpawnStartingAsteroids();
    SpawnNewShip();
}

//*******************************************************************
void Game::SetScreenShake(float deltaSeconds)
{
    if (m_screenShake != Vector2())
    {
        if (m_currentTimeForScreenShake < m_timeForScreenShake)
        {
            m_xboxController.RumbleController(10.0f, 10.0f);
            SetRandomScreenShake();
            m_currentTimeForScreenShake += deltaSeconds;
        }
        else
        {
            SetScreenShakeToZero();
            m_currentTimeForScreenShake = 0.0f;
        }
    }
}

//*******************************************************************
void Game::SetDisplayStarburstText(float deltaSeconds)
{
    if (m_playerHasStarBurst && (m_currentTimeForFlickeringStarburstText >= m_timeForFlickeringStarburstText))
    {
        m_displayStarburstText = !m_displayStarburstText;
        m_currentTimeForFlickeringStarburstText = 0.0f;
    }
    else if (m_playerHasStarBurst)
    {
        m_currentTimeForFlickeringStarburstText += deltaSeconds;
    }
}

//*******************************************************************
void Game::SetDisplayNewLifeText(float deltaSeconds)
{
    if ((m_ship == nullptr) && (m_currentTimeForFlickeringNewLifeText >= m_timeForFlickeringNewLifeText))
    {
        m_displayNewLifeText = !m_displayNewLifeText;
        m_currentTimeForFlickeringNewLifeText = 0.0f;
    }
    else if (m_ship == nullptr)
    {
        m_currentTimeForFlickeringNewLifeText += deltaSeconds;
    }
}

//*******************************************************************
void Game::SetDisplayStartGameText(float deltaSeconds)
{
    if (m_currentTimeForFlickeringStartGameText >= m_timeForFlickeringStartGameText)
    {
        m_displayStartGameText = !m_displayStartGameText;
        m_currentTimeForFlickeringStartGameText = 0.0f;
    }
    else
    {
        m_currentTimeForFlickeringStartGameText += deltaSeconds;
    }
}

//*******************************************************************
void Game::SetDisplayRestartGameText(float deltaSeconds)
{
    if (m_currentTimeForFlickeringRestartGameText >= m_timeForFlickeringRestartGameText)
    {
        m_displayRestartGameText = !m_displayRestartGameText;
        m_currentTimeForFlickeringRestartGameText = 0.0f;
    }
    else
    {
        m_currentTimeForFlickeringRestartGameText += deltaSeconds;
    }
}

//*******************************************************************
void Game::SetRandomScreenShake()
{
    float shakeX = GetRandomFloatInRange(-10.f, 10.f);
    float shakeY = GetRandomFloatInRange(-10.f, 10.f);
    m_screenShake = Vector2(shakeX, shakeY);
}

//*******************************************************************
void Game::SetScreenShakeToZero()
{
    Matrix4 newViewMatrix = g_theSimpleRenderer->matrix_data.view;
    newViewMatrix.Translate(Vector3(-1.f * m_screenShake.x, -1.f * m_screenShake.y, 0.f));
    g_theSimpleRenderer->SetViewMatrix(newViewMatrix);

    m_screenShake = Vector2();
}

//*******************************************************************
void Game::PlaySound(const std::string& soundFileName)
{
    SoundID soundId = g_theAudioSystem->CreateOrGetSound(soundFileName);
    g_theAudioSystem->PlaySoundID(soundId);
}

//*******************************************************************
void Game::PlayGameMusic(float deltaSeconds)
{
    if (m_currentTimeForGameMusicToPlay == 0.0f)
    {
        PlaySound("Data/Audio/Waves_in_Flight.wav");
        m_currentTimeForGameMusicToPlay += deltaSeconds;
    }
    else if (m_currentTimeForGameMusicToPlay < m_timeForGameMusicToPlay)
    {
        m_currentTimeForGameMusicToPlay += deltaSeconds;
    }
    else
    {
        m_currentTimeForGameMusicToPlay = 0.0f;
    }
}