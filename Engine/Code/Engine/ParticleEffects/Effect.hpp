#pragma once
#include "Engine/ParticleEffects/ParticleEmitter.hpp"
#include <map>

const int MAX_NUMBER_OF_PARTICLES_PER_FRAME = 50;

class Effect
{
public:
    std::string m_name;
    EMITTER_TYPE m_type;
    bool m_isPrewarmed;
    std::vector<ParticleEmitter*> m_particleEmitters;

    int m_currentNumberOfParticles;
    bool m_didAParticleReachMaxLifetime = false;

    Effect();
    Effect(std::string name, EMITTER_TYPE type, bool isPrewarmed);
    ~Effect();
    
    // stops the effect, but allows the existing particles to play out.
    // The system will destroy itself once it is finished (see is_finished)
    void Destroy(float deltaSeconds);

    // stops a particle effect, immediately destroying 
    // all of its existing particles.
    void DestroyImmediately();

    // resumes playing of an effect
    void Play(float deltaSeconds);
    void PlayOneOffEffect(float deltaSeconds);
    void PlayContinuousEffect(float deltaSeconds);

    // Should spawn particles and simulate the system up to the max
    // lifetime of the longest living particle
    // [optionally pass it a set time]
    void Prewarm();

    static Effect* GetKnownEffect(std::string name);
    static void PushKnownEffects();
};


static std::map<std::string, Effect*> s_effects;