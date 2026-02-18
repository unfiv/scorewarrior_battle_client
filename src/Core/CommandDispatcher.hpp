#pragma once

#include "Core/World.hpp"
#include "Core/IO/CommandParser.hpp"
#include "Core/Systems/Spatial.hpp"

#include "Core/Events/MapCreated.hpp"
#include "Core/Events/MarchStarted.hpp"

#include "Core/Commands/CommandRegistry.hpp"
#include "Core/Commands/CreateMap.hpp"
#include "Core/Commands/March.hpp"

namespace sw::core
{
    class CommandDispatcher
    {
    public:
        CommandDispatcher(World& world, io::CommandParser& parser)
        {
            parser.add<commands::CreateMap>([&world](auto command)
			{
                world.map = {command.width, command.height};
                world.getEvents().event(world.getTick(), events::MapCreated{command.width, command.height});
            });

            parser.add<commands::March>([&world](auto command)
            {
                // Check if the unit exists in our coordinate table
                if (auto it = world.positions.find(command.unitId); it != world.positions.end())
                {
                    const Position currentPos = it->second;
                    const Position targetPos = {command.targetX, command.targetY};

                    world.targetPositions[command.unitId] = targetPos;

                    world.getEvents().event(world.getTick(), events::MarchStarted{command.unitId, currentPos.x, currentPos.y, targetPos.x, targetPos.y});
                }
            });

            // Setup all the custom commands registered
			commands::CommandRegistry::setupAll(world, parser);
        }
    };
}
