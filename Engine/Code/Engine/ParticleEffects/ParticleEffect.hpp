#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/Rgba.hpp"
#include <vector>

struct ParticleState
{
    float m_time;
    Vector3 m_velocity;
};

class ParticleEffect
{
public:
    // My location/orientation in the world [may also
    // be a child of another object if I want to attach
    // the effect to something]
    Vector3 m_location;
    float m_rotation;
    Vector3 m_velocity;
    Vector3 m_acceleration;
    float m_size;
    Rgba m_tint;

    float m_maxLifetimeSeconds;
    float m_currentLifetimeSeconds;
    std::vector<ParticleState> m_states;

    ParticleEffect();
    ParticleEffect(Vector3 location, float rotation, Vector3 velocity, Vector3 acceleration, float size, float maxLifetimeSeconds, Rgba tint = Rgba::WHITE);
    ~ParticleEffect();

    void UdpateMovement(float deltaSeconds);
    void RunEuler(std::vector<ParticleState>& states, float deltaSeconds);
    void UdpateCurrentLifetimeSeconds(float deltaSeconds);
    void UpdateParticleTint();
};
