#include "Engine/ParticleEffects/EffectLoader.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

//*******************************************************************
EffectLoader::EffectLoader()
    : m_effectName("none")
    , m_effect(nullptr)
{}

//*******************************************************************
EffectLoader::EffectLoader(const tinyxml2::XMLElement& xmlElement)
{
    m_effectName = ParseXmlAttribute(xmlElement, "effect_name", m_effectName);

    std::string typeStr = "";
    typeStr = ParseXmlAttribute(xmlElement, "type_of_effect", typeStr);
    EMITTER_TYPE type = NONE;
    if (typeStr == "CONTINUOUS")
    {
        type = CONTINUOUS;
    }
    else
    {
        type = ONE_OFF;
    }

    bool isPrewarm = false;
    isPrewarm = ParseXmlAttribute(xmlElement, "effect_has_prewarm", isPrewarm);

    m_effect = new Effect(m_effectName, type, isPrewarm);
    PushEmitters(m_effect, xmlElement);
}

//*******************************************************************
EffectLoader::~EffectLoader()
{}

//*******************************************************************
Effect* EffectLoader::GetEffect(const std::string& effectName)
{
    return s_effects[effectName];
}

//*******************************************************************
void EffectLoader::PushEffect(Effect* effect)
{
    if (s_effects.find(effect->m_name) == s_effects.end())
    {
        s_effects[effect->m_name] = effect;
    }
    else
    {
        ASSERT_OR_DIE(true, "Muptile effects of the same type!");
    }
}

//*******************************************************************
void EffectLoader::PushEmitters(Effect* effect, const tinyxml2::XMLElement& XMLElement)
{
    const tinyxml2::XMLElement* emittersElement = XMLElement.FirstChildElement("emitters");

    for (auto xml_emitter = emittersElement->FirstChildElement("emitter"); xml_emitter != nullptr; xml_emitter = xml_emitter->NextSiblingElement())
    {
        //std::string m_name;
        //EMITTER_TYPE m_emitterType;
        std::string name = "";
        name = ParseXmlAttribute(*xml_emitter, "emitter_name", name);
        EMITTER_TYPE type = effect->m_type;

        //float m_lifetime;
        //bool m_isInfinite;
        const tinyxml2::XMLElement* lifetimeElement = xml_emitter->FirstChildElement("lifetime");
        bool isInfinite = false;
        isInfinite = ParseXmlAttribute(*lifetimeElement, "is_inifinite", isInfinite);
        float lifetime = 0.0;
        lifetime = ParseXmlAttribute(*lifetimeElement, "lifetime_seconds", lifetime);

        //int m_minimumNumberOfParticlesPerSecond;
        //int m_maximumNumberOfParticlesPerSecond;
        const tinyxml2::XMLElement* perSecondElement = xml_emitter->FirstChildElement("per_second");
        int minimumNumberOfParticlesPerSecond = 0;
        minimumNumberOfParticlesPerSecond = ParseXmlAttribute(*perSecondElement, "min_number_of_particles_per_second", minimumNumberOfParticlesPerSecond);
        int maximumNumberOfParticlesPerSecond = 0;
        maximumNumberOfParticlesPerSecond = ParseXmlAttribute(*perSecondElement, "max_number_of_particles_per_second", maximumNumberOfParticlesPerSecond);

        //float m_particleLifetime;
        const tinyxml2::XMLElement* particleLifetimeElement = xml_emitter->FirstChildElement("particle_lifetime");
        float particleLifetime = 0.0;
        particleLifetime = ParseXmlAttribute(*particleLifetimeElement, "p_lifetime_seconds", particleLifetime);

        //Vector3 m_location;
        const tinyxml2::XMLElement* locationElement = xml_emitter->FirstChildElement("location");
        Vector3 location = Vector3();
        location = ParseXmlAttribute(*locationElement, "world_location", location);

        //float m_minRotation;
        //float m_maxRotation;
        const tinyxml2::XMLElement* rotationElement = xml_emitter->FirstChildElement("rotation");
        float minRotation = 0.0f;
        minRotation = ParseXmlAttribute(*rotationElement, "min_rotation_degrees", minRotation);
        float maxRotation = 0.0f;
        maxRotation = ParseXmlAttribute(*rotationElement, "max_rotation_degrees", maxRotation);

        //Vector3 m_minVelocity;
        //Vector3 m_maxVelocity;
        const tinyxml2::XMLElement* velocityElement = xml_emitter->FirstChildElement("velocity");
        Vector3 minVelocity = Vector3();
        minVelocity = ParseXmlAttribute(*velocityElement, "min_velocity", minVelocity);
        Vector3 maxVelocity = Vector3();
        maxVelocity = ParseXmlAttribute(*velocityElement, "max_velocity", maxVelocity);

        //Vector3 m_acceleration;
        const tinyxml2::XMLElement* accelerationElement = xml_emitter->FirstChildElement("acceleration");
        Vector3 acceleration = Vector3();
        acceleration = ParseXmlAttribute(*accelerationElement, "acceleration_vector", acceleration);

        //Rgba m_tint;
        const tinyxml2::XMLElement* tintElement = xml_emitter->FirstChildElement("tint");
        Rgba tint = Rgba::WHITE;
        tint = ParseXmlAttribute(*tintElement, "tint_color", tint);

        //float m_size;
        const tinyxml2::XMLElement* sizeElement = xml_emitter->FirstChildElement("size");
        float size = 0.0f;
        size = ParseXmlAttribute(*sizeElement, "size_radius", size);

        //bool m_isPrewarm;
        const tinyxml2::XMLElement* preWarmElement = xml_emitter->FirstChildElement("prewarm");
        bool isPrewarm = false;
        isPrewarm = ParseXmlAttribute(*preWarmElement, "is_prewarm", isPrewarm);

        //int m_initialBurst;
        const tinyxml2::XMLElement* initialBurstElement = xml_emitter->FirstChildElement("initial_burst");
        int initialBurst = 0;
        initialBurst = ParseXmlAttribute(*initialBurstElement, "initial_number_particles", initialBurst);


        ParticleEmitter* currentParticleEmitter = new ParticleEmitter(name, type, isInfinite, lifetime, minimumNumberOfParticlesPerSecond, maximumNumberOfParticlesPerSecond, particleLifetime, location, minRotation, maxRotation, minVelocity, maxVelocity, acceleration, tint, size, isPrewarm, initialBurst);
        effect->m_particleEmitters.push_back(currentParticleEmitter);
    }
}

//*******************************************************************
void EffectLoader::ParseFromXML(const std::string& filenName)
{
    tinyxml2::XMLDocument xmlDoc;
    //tinyxml2::XMLError eResult = xmlDoc.LoadFile("Data/Gameplay/Tiles.xml");
    tinyxml2::XMLError eResult = xmlDoc.LoadFile(filenName.c_str());
    ASSERT_OR_DIE(eResult == tinyxml2::XML_SUCCESS, "Error in loading file from file, could not find Particles.xml file\n");

    auto root = xmlDoc.RootElement();
    ASSERT_OR_DIE(root != nullptr, "Error in loading file from file, could not find root\n");

    for (auto xml_effect = root->FirstChildElement("effect"); xml_effect != nullptr; xml_effect = xml_effect->NextSiblingElement())
    {
        EffectLoader* effectLoader = new EffectLoader(*xml_effect);
        EffectLoader::PushEffect(effectLoader->m_effect);
    }
}
