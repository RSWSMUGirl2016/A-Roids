#include "Engine/ParticleEffects/ParticleEmitter.hpp"
#include "Engine/Math/MathUtilities.hpp"

//*******************************************************************
ParticleEmitter::ParticleEmitter()
    : m_name("")
    , m_emitterType(NONE)
    , m_lifetime(0.0f)
    , m_isInfinite(false)
    , m_numberOfParticlesPerSecond(0)
    , m_particleLifetime(0.0f)
    , m_location(Vector3())
    , m_minRotation(0.0f)
    , m_maxRotation(0.0f)
    , m_minVelocity(Vector3())
    , m_maxVelocity(Vector3())
    , m_acceleration(Vector3())
    , m_tint(Rgba::WHITE)
    , m_size(0.0f)
    , m_isPrewarm(false)
    , m_initialBurst(0)
{}

//*******************************************************************
ParticleEmitter::ParticleEmitter(std::string name, EMITTER_TYPE emitterType, bool isInfinite, float emitterLifetimeSeconds, int minimumNumberOfParticlesPerSecond, int maximumNumberOfParticlesPerSecond, float individualParticleLifetime, Vector3 location, float minRotation, float maxRotation, Vector3 minVelocity, Vector3 maxVelocity, Vector3 acceleration, Rgba tint, float size, bool isPrewarm, int initialBurstOfParticles)
    : m_name(name)
    , m_emitterType(emitterType)
    , m_isInfinite(isInfinite)
    , m_lifetime(emitterLifetimeSeconds)
    , m_particleLifetime(individualParticleLifetime)
    , m_location(location)
    , m_minRotation(minRotation)
    , m_maxRotation(maxRotation)
    , m_minVelocity(minVelocity)
    , m_maxVelocity(maxVelocity)
    , m_acceleration(acceleration)
    , m_tint(tint)
    , m_size(size)
    , m_isPrewarm(isPrewarm)
    , m_initialBurst(initialBurstOfParticles)
{
    m_minimumNumberOfParticlesPerSecond = minimumNumberOfParticlesPerSecond;
    m_maximumNumberOfParticlesPerSecond = maximumNumberOfParticlesPerSecond;

    m_numberOfParticlesPerSecond = CalculateNumberOfParticlesPerFrame(minimumNumberOfParticlesPerSecond, maximumNumberOfParticlesPerSecond);

    if (m_initialBurst > 0)
    {
        PushNewParticlesForInitialBurst();
    }
    else
    {
        PushNewParticlesForFrame();
    }
}

//*******************************************************************
ParticleEmitter::~ParticleEmitter()
{
    for (ParticleEffect* particleEffect : m_particles)
        delete particleEffect;
    m_particles.clear();
}

//*******************************************************************
void ParticleEmitter::Play()
{
    //Removes particles from start of vector to free up room for new particles
    //Thinking of the mindset of continous generation of particles, and not a one-off
    /*if (((int)m_particles.size()) >= MAX_NUMBER_OF_PARTICLES_PER_FRAME)
    {
        m_particles.erase(m_particles.begin(), m_particles.begin() + m_numberOfParticlesPerSecond);
    }

    for (int particleIndex = 0; particleIndex < m_numberOfParticlesPerSecond; particleIndex++)
    {
        m_particles[particleIndex]->UdpateMovement(deltaSeconds);
    }

    PushNewParticlesForFrame();*/
}

//*******************************************************************
//**************************HELPERS**********************************
//*******************************************************************

//*******************************************************************
Vector3 ParticleEmitter::GetRandomVeloctiy(Vector3 minVelocity, Vector3 maxVelocity)
{
    float newVelocityX = GetRandomFloatInRange(minVelocity.x, maxVelocity.x);
    float newVelocityY = GetRandomFloatInRange(minVelocity.y, maxVelocity.y);
    float newVelocityZ = GetRandomFloatInRange(minVelocity.z, maxVelocity.z);
    return Vector3(newVelocityX, newVelocityY, newVelocityZ);
}

//*******************************************************************
float ParticleEmitter::GetRandomRotation(float minRotation, float maxRotation)
{
    return GetRandomFloatInRange(minRotation, maxRotation);
}

//*******************************************************************
void ParticleEmitter::PushNewParticle(Vector3 velocity, float rotation)
{
    ParticleEffect* particle = new ParticleEffect(m_location, rotation, velocity, m_acceleration, m_size, m_particleLifetime, m_tint);
    m_particles.push_back(particle);
}

//*******************************************************************
void ParticleEmitter::PushNewParticlesForFrame()
{
    //Set all particles to initial location
    for (int particleIndex = 0; particleIndex < m_numberOfParticlesPerSecond; particleIndex++)
    {
        Vector3 particleVelocity = GetRandomVeloctiy(m_minVelocity, m_maxVelocity);
        float particleRotation = GetRandomRotation(m_minRotation, m_maxRotation);
        PushNewParticle(particleVelocity, particleRotation);
    }
}

//*******************************************************************
void ParticleEmitter::PushNewParticlesForInitialBurst()
{
    //Set all particles to initial location
    for (int particleIndex = 0; particleIndex < m_initialBurst; particleIndex++)
    {
        Vector3 particleVelocity = GetRandomVeloctiy(m_minVelocity, m_maxVelocity);
        float particleRotation = GetRandomRotation(m_minRotation, m_maxRotation);
        PushNewParticle(particleVelocity, particleRotation);
    }
}

//*******************************************************************
int ParticleEmitter::CalculateNumberOfParticlesPerFrame(int minimumNumberOfParticlesPerSecond, int maximumNumberOfParticlesPerSecond)
{
    return GetRandomIntInRange(minimumNumberOfParticlesPerSecond, maximumNumberOfParticlesPerSecond);
}

//*******************************************************************
void ParticleEmitter::SetNewRandomSpawnRate()
{
    m_numberOfParticlesPerSecond = GetRandomIntInRange(m_minimumNumberOfParticlesPerSecond, m_maximumNumberOfParticlesPerSecond);
}
