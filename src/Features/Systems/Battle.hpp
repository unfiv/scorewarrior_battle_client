#pragma once

#include <random>
#include <vector>

#include "Core/World.hpp"
#include "Core/Systems/Spatial.hpp"

#include "Features/Domain/Health.hpp"
#include "Features/Domain/MeleeAttack.hpp"
#include "Features/Events/UnitAttacked.hpp"
#include "Features/Events/UnitAbilityUsed.hpp"
#include "Core/Events/UnitDied.hpp"

namespace sw::features::systems {

    using namespace sw::features::domain;

    class Battle {
    public:
        static void processUnit(core::World& world, uint32_t attackerId)
        {
            auto& healthMap = world.getComponent<Health>();
            auto& attackMap = world.getComponent<MeleeAttack>();

            // Ensure attacker exists and is functional
            if (healthMap.find(attackerId) == healthMap.end() || 
                attackMap.find(attackerId) == attackMap.end() || 
                healthMap[attackerId].hp == 0) {
                return;
            }

            auto attackerPos = world.positions[attackerId];
            std::vector<uint32_t> targets;
            core::systems::Spatial::findTargets(world, attackerId, attackerPos, targets);

            if (!targets.empty())
            {
                // Find first valid target
                uint32_t targetId = 0;
                for (uint32_t id : targets)
                {
                    if (healthMap.count(id) && healthMap[id].hp > 0)
                    {
                        targetId = id;
                        break;
                    }
                }

                if (targetId != 0)
                {
                    // Engagement prevents movement
                    world.restrictions.modify(attackerId, core::registry::restrictions::MOVE, 1);

                    executeAttack(world, attackerId, targetId);
                }
            }
        }

    private:
        static void executeAttack(core::World& world, uint32_t attackerId, uint32_t targetId)
        {
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(1, 1000);

            auto& attackerParams = world.getComponent<MeleeAttack>()[attackerId];
            auto& healthMap = world.getComponent<Health>();
            
            uint32_t damage = (dis(gen) <= attackerParams.chance) ? attackerParams.rending : attackerParams.strength;
            bool isAbility = (damage == attackerParams.rending && attackerParams.rending != attackerParams.strength);

            auto& targetHealth = healthMap[targetId];
            targetHealth.hp = (targetHealth.hp > damage) ? (targetHealth.hp - damage) : 0;

            if (isAbility)
            {
                world.getEvents().event(world.getTick(), events::UnitAbilityUsed{attackerId, "rending"});
            }
            else
            {
                world.getEvents().event(world.getTick(), events::UnitAttacked{attackerId, targetId, damage, targetHealth.hp});
            }
        }
    };
}