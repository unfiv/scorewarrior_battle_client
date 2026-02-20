#include "SpawnHunter.hpp"

#include "Core/World.hpp"
#include "Core/IO/CommandParser.hpp"
#include "Core/Commands/CommandRegistry.hpp"

namespace sw::features::commands
{
    void SpawnHunter::execute(core::World& world) const
    {
        (void)world;
    }
}

REGISTER_COMMAND([](sw::core::World& world, sw::core::io::CommandParser& parser) {
    parser.add<sw::features::commands::SpawnHunter>([&world](auto cmd) {
        cmd.execute(world);
    });
})
