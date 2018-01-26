#pragma once
#include "Engine/Core/XMLUtils.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Core/Rgba.hpp"
#include "Engine/ParticleEffects/Effect.hpp"
#include <map>
#include <vector>

class EffectLoader
{
public:
    std::string m_effectName;
    Effect* m_effect;

    EffectLoader();
    EffectLoader(const tinyxml2::XMLElement& xmlElement);
    ~EffectLoader();

    static Effect* GetEffect(const std::string& effectName);
    static void PushEffect(Effect* effect);
    void PushEmitters(Effect* effect, const tinyxml2::XMLElement& XMLElement);
    static void ParseFromXML(const std::string& filenName);
};
