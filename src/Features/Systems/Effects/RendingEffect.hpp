#pragma once

#include <any>

#include "Core/World.hpp"

#include "Features/Domain/Effects/EffectData.hpp"
#include "Features/Domain/Effects/EffectFactory.hpp"
#include "Features/Systems/Damage.hpp"

namespace sw::features::systems::effects
{
    class RendingEffect
    {
    public:
        static domain::effects::ActiveEffect create(uint32_t sourceUnitId, uint32_t damage)
        {
            return domain::effects::create(domain::effects::RendingEffectData{damage}, 1, sourceUnitId, apply);
        }

        static void apply(core::World& world, uint32_t targetId, domain::effects::ActiveEffect& effect)
        {
            auto& data = std::any_cast<domain::effects::RendingEffectData&>(effect.data);
            Damage::apply(world, effect.sourceUnitId, targetId, data.damage);
        }
    };
}
