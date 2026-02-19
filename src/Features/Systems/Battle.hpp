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
        static void update(core::World& world) {
            // Static random generator to persist across calls
            static std::random_device rd;
            static std::mt19937 gen(rd());
            static std::uniform_int_distribution<> dis(1, 1000);

            auto& healthMap = world.getComponent<Health>();
            auto& attackMap = world.getComponent<MeleeAttack>();

            // Collect potential attackers
            // We use a copy of IDs because a unit might reach 0 HP
            // within this loop if a counter-attack or multi-turn logic existed
            std::vector<uint32_t> attackerIds;
            for (auto const& [id, _] : attackMap)
            {
                attackerIds.push_back(id);
            }

            for (uint32_t attackerId : attackerIds)
            {
                // Ensure attacker still exists and is not dead
                if (healthMap.find(attackerId) == healthMap.end() || healthMap[attackerId].hp == 0)
                {
                    continue;
                }

                auto attackerPos = world.positions[attackerId];
                std::vector<uint32_t> targets;
                core::systems::Spatial::findTargets(world, attackerId, attackerPos, targets);
                if (!targets.empty())
                {
                    // BLOCK: Unit found a target, so it cannot move during this tick
                    world.restrictions.modify(attackerId, core::registry::restrictions::MOVE, 1);

                    // Pick the first valid alive target (legacy behavior)
                    uint32_t targetId = 0;
                    for (uint32_t id : targets)
                    {
                        if (healthMap.count(id) && healthMap[id].hp > 0)
                        {
                            targetId = id;
                            break;
                        }
                    }

                    if (targetId == 0) continue;

                    auto& attackerParams = attackMap[attackerId];
                    auto& targetHealth = healthMap[targetId];

                    // Ability vs Regular Attack
                    if (dis(gen) <= attackerParams.chance)
                    {
                        applyDamage(world, targetId, attackerParams.rending);
                        world.getEvents().event(world.getTick(), sw::features::events::UnitAbilityUsed{attackerId, "rending"});
                    }
                    else
                    {
                        applyDamage(world, targetId, attackerParams.strength);
                        world.getEvents().event(world.getTick(), sw::features::events::UnitAttacked{attackerId, targetId, attackerParams.strength, targetHealth.hp});
                    }
                }
            }
        }

    private:
        static void applyDamage(core::World& world, uint32_t targetId, uint32_t damage)
        {
            auto& healthMap = world.getComponent<Health>();
            auto& health = healthMap[targetId];
            health.hp = (health.hp > damage) ? (health.hp - damage) : 0;
        }
    };
}