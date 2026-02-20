#pragma once

#include "Core/World.hpp"

#include "Features/Domain/Health.hpp"
#include "Features/Events/UnitAttacked.hpp"

namespace sw::features::systems
{
    class Damage
    {
    public:
        static void apply(core::World& world, uint32_t attackerId, uint32_t targetId, uint32_t damage)
        {
            auto& healthMap = world.getComponent<domain::Health>();
            auto target = healthMap.find(targetId);
            if (target == healthMap.end() || target->second.hp == 0)
            {
                return;
            }

            target->second.hp = (target->second.hp > damage) ? (target->second.hp - damage) : 0;
            world.getEvents().event(world.getTick(), events::UnitAttacked{attackerId, targetId, damage, target->second.hp});
        }
    };
}

