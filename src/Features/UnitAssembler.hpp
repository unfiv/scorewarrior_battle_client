#pragma once

#include "Core/World.hpp"

#include "Features/Commands/SpawnSwordsman.hpp"
#include "Events/UnitSpawned.hpp"

namespace sw::game {
    class UnitAssembler {
    public:
        static void locate(core::World& world, uint32_t id, uint32_t x, uint32_t y) {
            world.positions[id] = {x, y};
        }

        static void spawnSwordsman(core::World& world, const io::SpawnSwordsman& cmd) {
            locate(world, cmd.unitId, cmd.x, cmd.y);
            
            // HP and others
            
            world.getEvents().event(world.getTick(), 
                io::UnitSpawned{cmd.unitId, "swordsman", cmd.x, cmd.y});
        }

        // Jun adds more here
    };
}