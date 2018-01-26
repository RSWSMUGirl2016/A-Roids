#pragma once
#include "Engine/Math/AABB2D.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/RHI/Texture2D.hpp"

enum SpriteAnimMode
{
	SPRITE_ANIM_MODE_PLAY_TO_END,
	SPRITE_ANIM_MODE_LOOPING,
	//SPRITE_ANIM_MODE_PINGPONG,
	NUM_SPRITE_ANIM_MODES
};

class SpriteAnimation
{
private:
	bool m_isFinished;
	bool m_isPlaying;
	SpriteSheet m_spriteSheet;
	float m_durationSeconds;
	float m_elapsedSeconds;
	SpriteAnimMode m_spriteAnimMode;
	int m_startSpriteIndex;
	int m_endSpriteIndex;
public:
	SpriteAnimation(const SpriteSheet& spriteSheet, float durationSeconds, SpriteAnimMode playbackMode, int startSpriteIndex, int endSpriteIndex);
public:
	void Update(float deltaSeconds);
	AABB2D GetCurrentTextureCoordinates() const;
	Texture* GetTexture() const;
    Texture2D* GetTexture2D() const;
	void Pause();
	void Resume();
	void Reset();
	bool IsFinished() const;
	bool IsPlaying() const;
	float GetDurationSeconds() const;
	float GetSecondsElapsed() const;
	float GetSecondsRemaining() const;
	float GetFractionElapsed() const;
	float GetFractionRemaining() const;
	void SetSecondsElapsed(float secondsElapsed);
	void SetFractionElapsed(float fractionElapsed);
};