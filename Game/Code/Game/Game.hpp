#pragma once
#include "Game/Asteroid.hpp"
#include "Game/Ship.hpp"
#include "Game/Bullet.hpp"
#include "Game/Starburst.hpp"
#include "Game/Explosion.hpp"
#include "Engine/Input/XboxController.hpp"

#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Console.hpp"
#include "Game/Camera3D.hpp"
#include "Engine/RHI/Texture2D.hpp"
#include "Engine/Core/Camera2D.hpp"
#include "Engine/Font/Font.hpp"
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Game/Alien.hpp"
#include "Game/Missile.hpp"

#include <vector>

const int MAX_ASTEROIDS = 1000;
const int MAX_BULLETS = 25;
const int NUM_LIVES_FOR_PLAYER = 3;

enum eGameState
{
    START_GAME_STATE,
    GAME_STATE,
    GAME_OVER_STATE,
};

class Game
{
public:
	Asteroid* m_asteroids[MAX_ASTEROIDS];
	Ship* m_ship;
    Bullet* m_bullets[MAX_BULLETS];
    Missile* m_missiles[MAX_BULLETS];
	bool m_IsQuitting;
	bool m_IsPaused;
	XboxController m_xboxController;
	int m_numberOfLivesForPlayer;
	Ship* m_currentLives[NUM_LIVES_FOR_PLAYER];
	Vector2 m_screenShake;
	Ship* m_healthPickup;
	Starburst* m_starburst;
	bool m_playerHasStarBurst;
    int m_score;
    Alien* m_alien;

    AABB2D m_playerShipTextureCoords;
    AABB2D m_playerShipSmallThrustTextureCoords;
    AABB2D m_playerShipFullThrustTextureCoords;
    AABB2D m_playerShipTurnRightTextureCoords;
    AABB2D m_playerShipTurnRightSmallThrustTextureCoords;
    AABB2D m_playerShipTurnRightFullThrustTextureCoords;
    AABB2D m_playerShipTurnLeftTextureCoords;
    AABB2D m_playerShipTurnLeftSmallThrustTextureCoords;
    AABB2D m_playerShipTurnLeftFullThrustTextureCoords;

    AABB2D m_enemyBulletTextureCoords;
    AABB2D m_enemyMissileTextureCoords;
    AABB2D m_playerBulletTextureCoords;
    AABB2D m_pickupTextureCoords;
    AABB2D m_asteroidTextureCoords;
    AABB2D m_starbustTextureCoords;
    AABB2D m_alienTextureCoords;
    AABB2D m_alienThrustingTextureCoords;
    Texture2D* m_bossAlienShipTexture;
    SpriteSheet* m_asteroidsSpriteSheet;
    SpriteSheet* m_spaceShipsSpriteSheet;
    SpriteSheet* m_explosionSpriteSheet;
    SpriteSheet* m_alienSpriteSheet;
    Texture2D* m_starfieldBackground;
    Texture2D* m_emptyStarfieldBackground;

    BitmapFont* m_bitmapFont;

private:
    float m_timeForScreenShake;
    float m_currentTimeForScreenShake;
    float m_timeForGameMusicToPlay;
    float m_currentTimeForGameMusicToPlay;
    std::vector<Explosion*> m_explosions;
    float m_timeForFlickeringStarburstText;
    float m_currentTimeForFlickeringStarburstText;
    bool m_displayStarburstText;
    float m_timeForFlickeringNewLifeText;
    float m_currentTimeForFlickeringNewLifeText;
    bool m_displayNewLifeText;
    eGameState m_currentGameState;
    float m_timeForFlickeringStartGameText;
    float m_currentTimeForFlickeringStartGameText;
    bool m_displayStartGameText;
    float m_timeForFlickeringRestartGameText;
    float m_currentTimeForFlickeringRestartGameText;
    bool m_displayRestartGameText;
    float m_currentTimeAlienExists;
    float m_timeSinceLastMissileWasSpawned;

public:
	Game();
	~Game();
    void SetUp();
    void Update(float deltaSeconds);
    void Render() const;
	void WrapAround();
	void OnKeyUp(char keyCode);
	void OnKeyDown(char keyCode);
	bool IsQuitting();

private:
    void UpdateStartGameState(float deltaSeconds);
    void UpdateGameState(float deltaSeconds);
    void UpdateGameOverState(float deltaSeconds);
    void RenderStartGameState() const;
    void RenderGameState() const;
    void RenderGameOverState() const;

private:
    void RenderSetup() const;
    void RenderExplosions() const;
    void RenderPlayerLives() const;

private:
    void UpdateAsteroids(float deltaSeconds);
	void SpawnNewAsteroid();
	void SpawnNewBossAsteroid();
	void DuplicateAsteroid(Asteroid* asteroid);
	void DestroyFirstAsteroid();
	void DestroyAsteroid(int asteroidIndex);
	void BreakAsteroid(Asteroid* asteroid, int asteroidIndex);
	void AddToAsteroidsList( Asteroid* newAsteroid );
	void DestroyAllAsteroids();
	void CreateNewWaveOfAsteroids();
	void BounceAsteroids();
    void SpawnStartingAsteroids();

private:
    void SpawnNewAlien();
    void DestroyAlien();
    void UpdateAlien(float deltaSeconds);

private:
	void SpawnNewShip();
	void DestroyShip();
	void GetDirectionForShip(char direction);
	void DirectionNoLongerBeingUsed(char direction);
	void SetShipOrientationOffXboxLeftJoyStick(float leftJoyStickAngle);
	void SetShipThrustOffXboxLeftJoyStick(float leftJoyStickR, float leftJoyStickAngle);

private:
	void RemoveLife();
	void RemoveAllLives();
	void SpawnNewLife();
	void DestroyHealthPickup();
    void AddLifeToLivesList();
    void SpawnStartingLives();

private:
	void SpawnNewStarburst();
	void DestroyStarburst();
	void UseStarburst();
	void AddStarburstForPlayerUse();

private:
	void AsteroidAndShipCollide();
	void AsteroidAndBulletCollide();
	void ShipAndHealthCollide();
    void ShipAndStarburstCollide();    
    void AlienAndBulletCollide();
    void AlienAndShipCollide();
    void ShipAndMissileCollide();

private:
    void UpdateBullets(float deltaSeconds);
	void SpawnNewBullet();
	void AddToBulletsList(Bullet* newBullet);
	void DestroyFirstBullet();
	void DestroyBullet(int bulletIndex);
    void DestroyAllBullets();

private:
    void UpdateMissiles(float deltaSeconds);
    void SpawnNewMissile();
    void AddToMissilesList(Missile* newMissile);
    void DestroyMissile(int missileIndex);
    void DestroyAllMissiles();
    void SpawnMissileWhileAlienExists(float deltaSeconds);

private:
    void DestroyAllExplosions();
    void GetOrCreateExplosion(bool isShip, Bullet* bullet, Missile* missile);
    void GenerateExplosion(bool isShip, Bullet* bullet, Missile* missile);
    void UpdateExplosions(float deltaSeconds);
    void DestroyExplosion(Vector2 entityPosition);

private:
    void ResetGame();
    void CalculateNewScore(float asteroidRadius);

private:
    void SetDisplayStarburstText(float deltaSeconds);
    void SetDisplayNewLifeText(float deltaSeconds);
    void SetDisplayStartGameText(float deltaSeconds);
    void SetDisplayRestartGameText(float deltaSeconds);

private:
    void SetScreenShake(float deltaSeconds);
	void SetRandomScreenShake();
	void SetScreenShakeToZero();

public:
    void PlaySound(const std::string& soundFileName);
    void PlayGameMusic(float deltaSeconds);
};

