#pragma once

#include <cstdint>

namespace sw::features::domain
{
    struct PoisonAbility
    {
        uint32_t chance{};
        uint32_t poison{};
    };
}

