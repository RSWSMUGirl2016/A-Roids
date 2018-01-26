#pragma once
#include "Engine/Renderer/SpriteAnimation.hpp"

class Explosion
{
public:
	SpriteAnimation m_spriteAnimation;
	float m_radius;
	Vector2 m_position;

public:
	Explosion(const SpriteAnimation& spriteAnimation, Vector2 position, float radius);
	~Explosion();
public:
	void Update(float deltaSeconds);
	void Render() const;
    void RenderDX() const;
};