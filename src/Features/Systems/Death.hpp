#pragma once

#include <vector>

#include "Core/World.hpp"
#include "Core/UnitManager.hpp"
#include "Features/Domain/Health.hpp"
#include "Features/Domain/MeleeAttack.hpp"

namespace sw::features::systems
{
    class Death
    {
    public:
        static void update(core::World& world)
        {
            auto& healthMap = world.getComponent<domain::Health>();
            std::vector<uint32_t> toDelete;

            // Identify all units that reached 0 HP during this tick
            for (auto const& [id, health] : healthMap)
            {
                if (health.hp == 0)
                {
                    toDelete.push_back(id);
                }
            }

            // Perform destruction and component cleanup
            for (uint32_t id : toDelete)
            {
                core::UnitManager::destroy(world, id, [&](uint32_t targetId)
                {
                    world.getComponent<domain::Health>().erase(targetId);
                    world.getComponent<domain::MeleeAttack>().erase(targetId);
                });
            }
        }
    };
}