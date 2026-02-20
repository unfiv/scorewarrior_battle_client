#pragma once

#include <any>
#include <cstdint>
#include <functional>
#include <typeindex>

namespace sw::core
{
    class World;
}

namespace sw::features::domain::effects
{
    struct ActiveEffect;
    using EffectApplyFn = std::function<void(core::World&, uint32_t, ActiveEffect&)>;

    struct ActiveEffect
    {
        std::any data;
        std::type_index dataType{typeid(void)};
        uint32_t remainingTicks{};
        uint32_t sourceUnitId{};
        EffectApplyFn applyFn;
    };
}
