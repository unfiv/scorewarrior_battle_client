#pragma once

#include <any>

#include "Core/World.hpp"

#include "Features/Domain/Effects/EffectData.hpp"
#include "Features/Domain/Effects/EffectFactory.hpp"
#include "Features/Systems/Damage.hpp"
#include "Features/Systems/Effects/EffectQueries.hpp"

namespace sw::features::systems::effects
{
    class PoisonEffect
    {
    public:
        static domain::effects::ActiveEffect create(uint32_t sourceUnitId, uint32_t totalDamage)
        {
            return domain::effects::create(domain::effects::PoisonEffectData{totalDamage, 0}, 5, sourceUnitId, apply);
        }

        static void apply(core::World& world, uint32_t targetId, domain::effects::ActiveEffect& effect)
        {
            constexpr uint32_t kPoisonTicks = 5;

            auto& data = std::any_cast<domain::effects::PoisonEffectData&>(effect.data);
            const uint32_t baseDamage = data.totalDamage / kPoisonTicks;
            const uint32_t remainder = data.totalDamage % kPoisonTicks;

            uint32_t tickDamage = baseDamage;
            if (data.appliedTicks < remainder)
            {
                ++tickDamage;
            }

            if (hasEffect<domain::effects::RendingEffectData>(world, targetId))
            {
                tickDamage *= 2;
            }

            if (tickDamage > 0)
            {
                Damage::apply(world, effect.sourceUnitId, targetId, tickDamage);
            }

            ++data.appliedTicks;
        }
    };
}
