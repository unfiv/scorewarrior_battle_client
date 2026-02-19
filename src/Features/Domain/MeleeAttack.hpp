#pragma once

#include <cstdint>

namespace sw::features::domain
{
    struct MeleeAttack
    { 
        uint32_t strength{};
        uint32_t chance{};
        uint32_t rending{};
    };
}