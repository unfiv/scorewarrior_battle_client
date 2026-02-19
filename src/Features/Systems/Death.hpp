#pragma once

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
            
            // Collect dead entities
            std::vector<uint32_t> toDelete;
            for (auto const& [id, health] : healthMap)
            {
                if (health.hp == 0)
                {
                    toDelete.push_back(id);
                }
            }

            // Cleanup all component maps and core data
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