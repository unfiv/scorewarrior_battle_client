#pragma once

#include "Core/Commands/CommandRegistry.hpp"
#include "Core/Commands/CreateMap.hpp"
#include "Core/Commands/March.hpp"
#include "Core/IO/CommandParser.hpp"
#include "Core/World.hpp"

#include <istream>

namespace sw::core
{
    class CommandProcessor
    {
    public:
        explicit CommandProcessor(World& world)
            : _world(world)
        {
            _parser.add<commands::CreateMap>();
            _parser.add<commands::March>();
            commands::CommandRegistry::setupAll(_parser);
        }

        void process(std::istream& stream)
        {
            auto commands = _parser.parse(stream);
            for (const auto& command : commands)
            {
                command->execute(_world);
            }
        }

    private:
        World& _world;
        io::CommandParser _parser;
    };
}
