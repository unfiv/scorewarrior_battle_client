#pragma once

#include <functional>
#include <vector>

namespace sw::core::io
{
    class CommandParser;
}

namespace sw::core::commands
{
    using CommandSetupFn = std::function<void(io::CommandParser&)>;

    class CommandRegistry
    {
    public:
        static void add(CommandSetupFn fn)
        {
            getInitializers().push_back(std::move(fn));
        }

        static void setupAll(sw::core::io::CommandParser& parser)
        {
            for (const auto& fn : getInitializers())
            {
                fn(parser);
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
