#include "Engine/ParticleEffects/Effect.hpp"

//*******************************************************************
Effect::Effect()
    : m_name("")
    , m_type(NONE)
    , m_currentNumberOfParticles(0)
    , m_isPrewarmed(false)
{}

//*******************************************************************
Effect::Effect(std::string name, EMITTER_TYPE type, bool isPrewarmed)
    : m_name(name)
    , m_type(type)
    , m_currentNumberOfParticles(0)
    , m_isPrewarmed(isPrewarmed)
{}

//*******************************************************************
Effect::~Effect()
{
    for (ParticleEmitter* particleEmitter : m_particleEmitters)
        delete particleEmitter;
    m_particleEmitters.clear();
}

//*******************************************************************
void Effect::Destroy(float deltaSeconds)
{
    for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
    {
        ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
        //particleEmitter->Play(deltaSeconds);
        std::vector<ParticleEffect*> particles = particleEmitter->m_particles;

        //Removes a particle when it reaches the end of it's own lifetime
        for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
        {
            ParticleEffect* particle = particles[particleIndex];
            if (particle->m_currentLifetimeSeconds <= 0.0f)
            {
                particles.erase(particles.begin() + particleIndex);
            }
        }
        particleEmitter->m_particles = particles;

        //Updates the movement of the current particles
        for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
        {
            ParticleEffect* particle = particles[particleIndex];
            particle->UdpateCurrentLifetimeSeconds(deltaSeconds);
            particle->UdpateMovement(deltaSeconds);
            particle->UpdateParticleTint();
        }
    }

    //Does memory cleanup for emitters
    bool allEmittersEmpty = true;
    for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
    {
        ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
        std::vector<ParticleEffect*> particles = particleEmitter->m_particles;
        if ((int)particles.size() > 0)
            allEmittersEmpty = false;
    }

    if (allEmittersEmpty)
    {
        for (ParticleEmitter* particleEmitter : m_particleEmitters)
            delete particleEmitter;
        m_particleEmitters.clear();
    }
}

//*******************************************************************
void Effect::DestroyImmediately()
{
    for (ParticleEmitter* particleEmitter : m_particleEmitters)
        delete particleEmitter;
    m_particleEmitters.clear();
}

//*******************************************************************
void Effect::Play(float deltaSeconds)
{
    if (m_isPrewarmed && !m_didAParticleReachMaxLifetime)
    {
        Prewarm();
        return;
    }

    switch (m_type)
    {
    case ONE_OFF: PlayOneOffEffect(deltaSeconds); break;
    case CONTINUOUS: PlayContinuousEffect(deltaSeconds); break;
    }

}

//*******************************************************************
void Effect::PlayOneOffEffect(float deltaSeconds)
{
    //Removes particles from start of vector to free up room for new particles
    //Thinking of the mindset of continuous generation of particles, and not a one-off
    for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
    {
        ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
        //particleEmitter->Play(deltaSeconds);
        std::vector<ParticleEffect*> particles = particleEmitter->m_particles;

        //Removes a particle when it reaches the end of it's own lifetime
        for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
        {
            ParticleEffect* particle = particles[particleIndex];
            if (particle->m_currentLifetimeSeconds <= 0.0f)
            {
                particles.erase(particles.begin() + particleIndex);
            }
        }
        particleEmitter->m_particles = particles;

        //Updates the movement of the current particles
        for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
        {
            ParticleEffect* particle = particles[particleIndex];
            particle->UdpateCurrentLifetimeSeconds(deltaSeconds);
            particle->UdpateMovement(deltaSeconds);
            particle->UpdateParticleTint();
        }
    }

    bool allEmittersEmpty = true;
    for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
    {
        ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
        std::vector<ParticleEffect*> particles = particleEmitter->m_particles;
        if ((int)particles.size() > 0)
            allEmittersEmpty = false;
    }

    if (allEmittersEmpty)
    {
        for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
        {
            ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
            particleEmitter->SetNewRandomSpawnRate();
            particleEmitter->PushNewParticlesForFrame();
            m_currentNumberOfParticles += particleEmitter->m_numberOfParticlesPerSecond;
        }
    }
}

//*******************************************************************
void Effect::PlayContinuousEffect(float deltaSeconds)
{
    //Removes particles from start of vector to free up room for new particles
    //Thinking of the mindset of continuous generation of particles, and not a one-off
    for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
    {
        ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
        //particleEmitter->Play(deltaSeconds);
        std::vector<ParticleEffect*> particles = particleEmitter->m_particles;

        //Removes a particle when it reaches the end of it's own lifetime
        for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
        {
            ParticleEffect* particle = particles[particleIndex];
            if (particle->m_currentLifetimeSeconds <= 0.0f)
            {
                particles.erase(particles.begin() + particleIndex);
            }
        }
        particleEmitter->m_particles = particles;

        //Updates the movement of the current particles
        for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
        {
            ParticleEffect* particle = particles[particleIndex];
            particle->UdpateCurrentLifetimeSeconds(deltaSeconds);
            particle->UdpateMovement(deltaSeconds);
            particle->UpdateParticleTint();
        }

        //Adds new particles
        particleEmitter->SetNewRandomSpawnRate();
        particleEmitter->PushNewParticlesForFrame();
        m_currentNumberOfParticles += particleEmitter->m_numberOfParticlesPerSecond;
    }
}

//*******************************************************************
void Effect::Prewarm()
{
    float deltaSeconds = 0.05f;

    while (!m_didAParticleReachMaxLifetime)
    {
        for (int emitterIndex = 0; emitterIndex < (int)m_particleEmitters.size(); ++emitterIndex)
        {
            ParticleEmitter* particleEmitter = m_particleEmitters[emitterIndex];
            //particleEmitter->Play(deltaSeconds);
            std::vector<ParticleEffect*> particles = particleEmitter->m_particles;

            //Removes a particle when it reaches the end of it's own lifetime
            for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
            {
                ParticleEffect* particle = particles[particleIndex];
                if (particle->m_currentLifetimeSeconds <= 0.0f)
                {
                    m_didAParticleReachMaxLifetime = true;
                    return;
                }
            }
            particleEmitter->m_particles = particles;

            //Updates the movement of the current particles
            for (int particleIndex = 0; particleIndex < (int)particles.size(); particleIndex++)
            {
                ParticleEffect* particle = particles[particleIndex];
                particle->UdpateCurrentLifetimeSeconds(deltaSeconds);
                particle->UdpateMovement(deltaSeconds);
                particle->UpdateParticleTint();
            }
        }
    }
}

//*******************************************************************
Effect* Effect::GetKnownEffect(std::string name)
{
    if (s_effects.find(name) == s_effects.end()) {
        // not found
        return nullptr;
    }
    else {
        // found
        return s_effects[name];
    }
}

//*******************************************************************
void Effect::PushKnownEffects()
{
    //Continuous Effect
    Effect* effect = new Effect("test effect", CONTINUOUS, false);
    ParticleEmitter* particleEmitterOne = new ParticleEmitter("test emitter", CONTINUOUS, true, 0.0f, 5, 10, 0.5f, Vector3(), 30.0f, 90.0f, Vector3(10.f, 10.f, 10.f), Vector3(100.f, 100.f, 100.f), Vector3(), Rgba(255, 0, 0, 255), 0.05f, false, 0);
    effect->m_particleEmitters.push_back(particleEmitterOne);
    ParticleEmitter* particleEmiiterTwo = new ParticleEmitter("test emitter 2", CONTINUOUS, true, 0.0f, 5, 10, 0.5f, Vector3(), 30.0f, 90.0f, Vector3(-100.f, -100.f, -100.f), Vector3(-10.f, -10.f, -10.f), Vector3(), Rgba(0, 0, 255, 255), 0.05f, false, 0);
    effect->m_particleEmitters.push_back(particleEmiiterTwo);

    s_effects[effect->m_name] = effect;


    //One-off Effect
    Effect* effectTwo = new Effect("one-off effect", ONE_OFF, true);
    ParticleEmitter* particleEmitterThree = new ParticleEmitter("test emitter 3", ONE_OFF, true, 0.0f, 5, 10, 0.5f, Vector3(), 30.0f, 90.0f, Vector3(10.f, 10.f, 10.f), Vector3(100.f, 100.f, 100.f), Vector3(), Rgba(255, 0, 0, 255), 0.05f, false, 0);
    effectTwo->m_particleEmitters.push_back(particleEmitterThree);

    s_effects[effectTwo->m_name] = effectTwo;
}
