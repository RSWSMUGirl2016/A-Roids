#include "Engine/Renderer/SpriteAnimation.hpp"

//*******************************************************************
SpriteAnimation::SpriteAnimation(const SpriteSheet& spriteSheet, float durationSeconds, SpriteAnimMode playbackMode, int startSpriteIndex, int endSpriteIndex)
	: m_spriteSheet(spriteSheet)
	, m_durationSeconds(durationSeconds)
	, m_spriteAnimMode(playbackMode)
	, m_startSpriteIndex(startSpriteIndex)
	, m_endSpriteIndex(endSpriteIndex)
	, m_isFinished(false)
	, m_isPlaying(true)
	, m_elapsedSeconds(0.f) 
{
}

//*******************************************************************
void SpriteAnimation::Update(float deltaSeconds)
{
	if (!m_isPlaying)
	{
		deltaSeconds = 0.f;
	}
		
	if (m_spriteAnimMode == SPRITE_ANIM_MODE_LOOPING && m_elapsedSeconds > m_durationSeconds)
	{
		Reset();
	}
		
	if (m_spriteAnimMode == SPRITE_ANIM_MODE_PLAY_TO_END && m_elapsedSeconds > m_durationSeconds)
	{
 		m_isFinished = true;
		m_isPlaying = false;
	}
	
	m_elapsedSeconds += deltaSeconds;
}

//*******************************************************************
AABB2D SpriteAnimation::GetCurrentTextureCoordinates() const
{
  	int numTilesInAnimation = (m_endSpriteIndex - m_startSpriteIndex) + 1;
	float durationPerTile = m_durationSeconds / (float)numTilesInAnimation;

	int tileIndex = (int)(m_elapsedSeconds / durationPerTile) + m_startSpriteIndex;
	if (tileIndex > m_endSpriteIndex)
		tileIndex = m_endSpriteIndex;

	return m_spriteSheet.GetTexCoordsForSpriteIndex(tileIndex);
}

//*******************************************************************
Texture* SpriteAnimation::GetTexture() const
{
	return m_spriteSheet.GetTexture();
}

//*******************************************************************
Texture2D* SpriteAnimation::GetTexture2D() const
{
    return m_spriteSheet.GetTexture2D();
}

//*******************************************************************
void SpriteAnimation::Pause()
{
	m_isPlaying = false;
}

//*******************************************************************
void SpriteAnimation::Resume()
{
	m_isPlaying = true;
}

//*******************************************************************
void SpriteAnimation::Reset()
{
	m_elapsedSeconds = 0.f;
}

//*******************************************************************
bool SpriteAnimation::IsFinished() const
{
	return m_isFinished;
}

//*******************************************************************
bool SpriteAnimation::IsPlaying() const
{
	return m_isPlaying;
}

//*******************************************************************
float SpriteAnimation::GetDurationSeconds() const
{
	return m_durationSeconds;
}

//*******************************************************************
float SpriteAnimation::GetSecondsElapsed() const
{
	return m_elapsedSeconds;
}

//*******************************************************************
float SpriteAnimation::GetSecondsRemaining() const
{
	return m_durationSeconds - m_elapsedSeconds;
}

//*******************************************************************
float SpriteAnimation::GetFractionElapsed() const
{
	return m_elapsedSeconds / m_durationSeconds;
}

//*******************************************************************
float SpriteAnimation::GetFractionRemaining() const
{
	return (m_durationSeconds - m_elapsedSeconds) / m_durationSeconds;
}

//*******************************************************************
void SpriteAnimation::SetSecondsElapsed(float secondsElapsed)
{
	m_elapsedSeconds = secondsElapsed;
}

//*******************************************************************
void SpriteAnimation::SetFractionElapsed(float fractionElapsed)
{
	m_elapsedSeconds = fractionElapsed * m_durationSeconds;
}

