#include "Engine/ParticleEffects/ParticleEffect.hpp"
#include "Engine//Math/MathUtilities.hpp"

//*******************************************************************
ParticleEffect::ParticleEffect()
    : m_location(Vector3())
    , m_rotation(0.0f)
    , m_velocity(Vector3())
    , m_acceleration(Vector3())
    , m_size(0.0f)
    , m_tint(Rgba::WHITE)
    , m_maxLifetimeSeconds(0.0f)
    , m_currentLifetimeSeconds(0.0f)
{}

//*******************************************************************
ParticleEffect::ParticleEffect(Vector3 location, float rotation, Vector3 velocity, Vector3 acceleration, float size, float maxLifetimeSeconds, Rgba tint)
    : m_location(location)
    , m_rotation(rotation)
    , m_velocity(velocity)
    , m_acceleration(acceleration)
    , m_size(size)
    , m_tint(tint)
    , m_maxLifetimeSeconds(maxLifetimeSeconds)
    , m_currentLifetimeSeconds(maxLifetimeSeconds)
{}

//*******************************************************************
ParticleEffect::~ParticleEffect()
{}

//*******************************************************************
void ParticleEffect::UdpateMovement(float deltaSeconds)
{
    //Needed to double check the math for this!
    /*Vector3 acceleration = Vector3(m_acceleration.x * CosDegrees(m_rotation), m_acceleration.y * SinDegrees(m_rotation), 0.0f);
    
    m_velocity.x += (acceleration.x * deltaSeconds);
    m_velocity.y += (acceleration.y * deltaSeconds);
    m_velocity.z += (acceleration.z * deltaSeconds);

    m_location.x += (m_velocity.x * deltaSeconds);
    m_location.y += (m_velocity.y * deltaSeconds);
    m_location.z += (m_velocity.z * deltaSeconds);*/

    RunEuler(m_states, deltaSeconds);
    m_location.x += (m_velocity.x * deltaSeconds);
    m_location.y += (m_velocity.y * deltaSeconds);
    m_location.z += (m_velocity.z * deltaSeconds);
}

//*******************************************************************
void ParticleEffect::RunEuler(std::vector<ParticleState>& states, float deltaSeconds)
{
    const int iterations((int)(m_currentLifetimeSeconds / deltaSeconds));
    if (iterations <= 0)
    {
        m_velocity = Vector3();
        return;
    }

    states.resize(iterations);
    states[0].m_time = 0.0f;
    states[0].m_velocity = m_velocity;

    for (int particleStateIndex = 1; particleStateIndex < iterations; particleStateIndex++)
    {
        ParticleState& newParticleState = states[particleStateIndex];
        ParticleState& previousParticleState = states[particleStateIndex - 1];
        Vector3& newVelocity = newParticleState.m_velocity;
        Vector3& previousVelocity = previousParticleState.m_velocity;

        newParticleState.m_time = previousParticleState.m_time + deltaSeconds;

        newVelocity.x = (m_acceleration.x * deltaSeconds) + (1 - deltaSeconds) * previousVelocity.x;
        newVelocity.y = (m_acceleration.y * deltaSeconds) + (1 - deltaSeconds) * previousVelocity.y;
        newVelocity.z = (m_acceleration.z * deltaSeconds) + (1 - deltaSeconds) * previousVelocity.z;
    }

    m_velocity = states[(int)states.size() - 1].m_velocity;
}

//*******************************************************************
void ParticleEffect::UdpateCurrentLifetimeSeconds(float deltaSeconds)
{
    m_currentLifetimeSeconds -= deltaSeconds;
}

//*******************************************************************
void ParticleEffect::UpdateParticleTint()
{
    float tintPercentage = (m_currentLifetimeSeconds / m_maxLifetimeSeconds) * 100.f;
    float newAlphaValue = RangeMapFloat(tintPercentage, 0.0f, 1.0f, 0.0f, 255.0f);
    m_tint.a = (unsigned char)newAlphaValue;
}