#pragma once

#include <vector>

#include "Core/World.hpp"
#include "Core/UnitManager.hpp"
#include "Features/Domain/Health.hpp"
#include "Features/Domain/Melee.hpp"
#include "Features/Domain/RendingAbility.hpp"
#include "Features/Domain/Ranged.hpp"
#include "Features/Domain/PoisonAbility.hpp"
#include "Features/Domain/Effects/EffectList.hpp"

namespace sw::features::systems
{
    class Death
    {
    public:
        static void processUnit(core::World& world, uint32_t id)
        {
            auto& healthMap = world.getComponent<domain::Health>();
            auto health = healthMap.find(id);
			if (health != healthMap.end())
            {
                if (health->second.hp == 0)
					destroy(world, id);
            }
        }

        static void update(core::World& world)
        {
            auto& healthMap = world.getComponent<domain::Health>();
            std::vector<uint32_t> toDelete;
            for (auto const& [id, health] : healthMap)
            {
                if (health.hp == 0)
                    toDelete.push_back(id);
            }

            for (uint32_t id : toDelete)
                destroy(world, id);                
        }

	private:
        static void destroy(core::World& world, uint32_t id)
        {
            core::UnitManager::destroy(world, id, [&](uint32_t targetId)
            {
                world.getComponent<domain::Health>().erase(targetId);
                world.getComponent<domain::Melee>().erase(targetId);
                world.getComponent<domain::RendingAbility>().erase(targetId);
                world.getComponent<domain::Ranged>().erase(targetId);
                world.getComponent<domain::PoisonAbility>().erase(targetId);
                world.getComponent<domain::effects::EffectList>().erase(targetId);
            });
        }
    };
}
