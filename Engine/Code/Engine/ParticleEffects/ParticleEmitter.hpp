#pragma once

#include "Engine/ParticleEffects/ParticleEffect.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/Rgba.hpp"
#include <string>
#include <vector>


enum EMITTER_TYPE
{
    NONE,
    ONE_OFF,
    CONTINUOUS,
    NUM_PARTICLE_TYPES
};

class ParticleEmitter
{
public:
    std::string m_name;
    EMITTER_TYPE m_emitterType;
    
    float m_lifetime;
    bool m_isInfinite;
    
    int m_minimumNumberOfParticlesPerSecond;
    int m_maximumNumberOfParticlesPerSecond;
    int m_numberOfParticlesPerSecond;
    float m_particleLifetime;    
    
    Vector3 m_location;
    float m_minRotation;
    float m_maxRotation;

    Vector3 m_minVelocity;
    Vector3 m_maxVelocity;
    Vector3 m_acceleration;

    Rgba m_tint;
    float m_size;
    bool m_isPrewarm;
    int m_initialBurst;

    std::vector<ParticleEffect*> m_particles;

public:
    ParticleEmitter();
    ParticleEmitter(std::string name, EMITTER_TYPE emitterType, bool isInfinite, float emitterLifetimeSeconds, int minimumNumberOfParticlesPerSecond, int maximumNumberOfParticlesPerSecond, float individualParticleLifetime, Vector3 location, float minRotation, float maxRotation, Vector3 minVelocity, Vector3 maxVelocity, Vector3 acceleration, Rgba tint, float size, bool isPrewarm, int initialBurstOfParticles);
    ~ParticleEmitter();
    
    // resumes playing of an effect
    void Play();

    // Helper Functions for setting velocity, rotation, etc.
    Vector3 GetRandomVeloctiy(Vector3 minVelocity, Vector3 maxVelocity);
    float GetRandomRotation(float minRotation, float maxRotation);

    void PushNewParticle(Vector3 velocity, float rotation);
    void PushNewParticlesForFrame();
    void PushNewParticlesForInitialBurst();
    int CalculateNumberOfParticlesPerFrame(int minimumNumberOfParticlesPerSecond, int maximumNumberOfParticlesPerSecond);
    void SetNewRandomSpawnRate();
};