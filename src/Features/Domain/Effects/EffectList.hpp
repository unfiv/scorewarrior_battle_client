#pragma once

#include <vector>

#include "Features/Domain/Effects/ActiveEffect.hpp"

namespace sw::features::domain::effects
{
    struct EffectList
    {
        std::vector<ActiveEffect> active;
    };
}
