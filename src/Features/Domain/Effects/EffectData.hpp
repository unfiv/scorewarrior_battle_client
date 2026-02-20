#pragma once

#include <cstdint>

namespace sw::features::domain::effects
{
    struct RendingEffectData
    {
        uint32_t damage{};
    };

    struct PoisonEffectData
    {
        uint32_t totalDamage{};
        uint32_t appliedTicks{};
    };
}
