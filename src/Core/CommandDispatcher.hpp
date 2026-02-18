#pragma once

#include "Core/Commands/CommandRegistry.hpp"
#include "Core/IO/CommandParser.hpp"
#include "Core/World.hpp"
#include "Core/Commands/CreateMap.hpp" 
#include "Core/Events/MapCreated.hpp"

namespace sw::core
{
    class CommandDispatcher
    {
    public:
        CommandDispatcher(World& world, io::CommandParser& parser)
        {
            parser.add<commands::CreateMap>([&world](auto cmd)
			{
                world.map = {cmd.width, cmd.height};
                world.getEvents().event(world.getTick(), events::MapCreated{cmd.width, cmd.height});
            });

            // Setup all the custom commands registered
			commands::CommandRegistry::setupAll(world, parser);
        }
    };
}
