#include "SpawnSwordsman.hpp"

#include "Core/World.hpp"
#include "Core/UnitManager.hpp"
#include "Core/Commands/CommandRegistry.hpp"
#include "Core/IO/CommandParser.hpp"

namespace sw::features::commands
{
    void SpawnSwordsman::execute(core::World& world) const
    {
        core::UnitManager::spawn(world, unitId, "swordsman", {x, y}, [&]()
        {
            //world.unitsHp[unitId] = hp;
        });
    }
}

REGISTER_COMMAND([](sw::core::World& world, sw::core::io::CommandParser& parser) {
    parser.add<sw::features::commands::SpawnSwordsman>([&world](auto cmd) {
        cmd.execute(world);
    });
})
