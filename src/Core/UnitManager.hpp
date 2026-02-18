#pragma once

#include <string>
#include <functional>

#include "Core/World.hpp"
#include "Core/IO/EventSystem.hpp"
#include "Core/Events/UnitSpawned.hpp"
#include "Core/Events/UnitDied.hpp"

namespace sw::core
{
    class UnitManager
    {
    public:
        template<typename F>
        static void spawn(World& world, uint32_t id, const std::string& type, Position pos, F&& setup)
        {
            world.positions[id] = pos;
            setup(); 
            world.getEvents().event(world.getTick(), events::UnitSpawned{id, type, pos.x, pos.y});
        }

        static void destroy(World& world, uint32_t id)
        {
            if (world.positions.erase(id)) {
                // Will add HP cleanup later here: world.health.erase(id);
				world.getEvents().event(world.getTick(), events::UnitDied{id});
            }
        }
    };
}
