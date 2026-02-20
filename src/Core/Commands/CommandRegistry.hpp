#pragma once
#include <functional>
#include <vector>

namespace sw::core
{
    class World;

    namespace io
    {
        class CommandParser;
    }
}

namespace sw::core::commands
{
    using CommandSetupFn = std::function<void(World&, io::CommandParser&)>;

    class CommandRegistry
    {
    public:
        static void add(CommandSetupFn fn)
        {
            getInitializers().push_back(fn);
        }

        static void setupAll(World& world, sw::core::io::CommandParser& parser)
        {
            for (const auto& fn : getInitializers())
            {
                fn(world, parser);
            }
        }

    private:
        static std::vector<CommandSetupFn>& getInitializers()
        {
            static std::vector<CommandSetupFn> initializers;
            return initializers;
        }
    };
}

#define REGISTER_COMMAND(setupBlock) \
static bool _cmd_reg_##__LINE__ = []() { \
    ::sw::core::commands::CommandRegistry::add(setupBlock); \
    return true; \
}();