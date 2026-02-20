#pragma once

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

#include "Core/World.hpp"
#include "Core/Systems/Spatial.hpp"

#include "Features/Domain/Health.hpp"
#include "Features/Domain/Ranged.hpp"
#include "Features/Domain/PoisonAbility.hpp"
#include "Features/Events/UnitAbilityUsed.hpp"
#include "Features/Systems/Damage.hpp"
#include "Features/Systems/Effects.hpp"
#include "Features/Systems/Effects/PoisonEffect.hpp"

namespace sw::features::systems
{
    class RangedAttack
    {
    public:
        static void processUnit(core::World& world, uint32_t attackerId)
        {
            if (!world.restrictions.isAllowed(attackerId, core::registry::restrictions::ATTACK))
            {
                return;
            }

            auto& healthMap = world.getComponent<domain::Health>();
            auto& rangedMap = world.getComponent<domain::Ranged>();

            if (healthMap.find(attackerId) == healthMap.end() ||
                rangedMap.find(attackerId) == rangedMap.end() ||
                healthMap[attackerId].hp == 0)
            {
                return;
            }

            auto attackerPos = world.positions[attackerId];

            if (hasAdjacentUnits(world, attackerId, attackerPos))
            {
                return;
            }

            std::vector<uint32_t> targets;
            for (const auto& [targetId, targetPos] : world.positions)
            {
                if (targetId == attackerId)
                {
                    continue;
                }

                auto targetHealth = healthMap.find(targetId);
                if (targetHealth == healthMap.end() || targetHealth->second.hp == 0)
                {
                    continue;
                }

                uint32_t distance = chebyshevDistance(attackerPos, targetPos);
                if (distance >= 2 && distance <= rangedMap[attackerId].range)
                {
                    targets.push_back(targetId);
                }
            }

            if (targets.empty())
            {
                return;
            }

            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<size_t> targetDistribution(0, targets.size() - 1);
            uint32_t targetId = targets[targetDistribution(gen)];

            world.restrictions.modify(attackerId, core::registry::restrictions::MOVE, 1);
            world.restrictions.modify(attackerId, core::registry::restrictions::ATTACK, 1);

            static std::uniform_int_distribution<> chanceRoll(1, 1000);

            auto& poisonAbilities = world.getComponent<domain::PoisonAbility>();
            if (auto ability = poisonAbilities.find(attackerId); ability != poisonAbilities.end() &&
                chanceRoll(gen) <= ability->second.chance)
            {
                Effects::addEffect(world, targetId, effects::PoisonEffect::create(attackerId, ability->second.poison));
                world.getEvents().event(world.getTick(), events::UnitAbilityUsed{attackerId, "poison"});
                return;
            }

            const auto damage = rangedMap[attackerId].agility;
            Damage::apply(world, attackerId, targetId, damage);
        }

    private:
        static uint32_t chebyshevDistance(core::Position lhs, core::Position rhs)
        {
            auto dx = std::abs(static_cast<int>(lhs.x) - static_cast<int>(rhs.x));
            auto dy = std::abs(static_cast<int>(lhs.y) - static_cast<int>(rhs.y));
            return static_cast<uint32_t>(std::max(dx, dy));
        }

        static bool hasAdjacentUnits(core::World& world, uint32_t attackerId, core::Position attackerPos)
        {
            auto& healthMap = world.getComponent<domain::Health>();
            std::vector<uint32_t> adjacent;
            core::systems::Spatial::findTargets(world, attackerId, attackerPos, adjacent);
            return std::any_of(adjacent.begin(), adjacent.end(), [&](uint32_t targetId)
            {
                return healthMap.count(targetId) && healthMap[targetId].hp > 0;
            });
        }
    };
}
