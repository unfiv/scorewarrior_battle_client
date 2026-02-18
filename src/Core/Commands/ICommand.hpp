#pragma once

namespace sw::core
{
    class World;
}

namespace sw::core::commands
{
    struct ICommand
    {
        virtual ~ICommand() = default;
        virtual void execute(sw::core::World& world) const = 0;
    };
}
