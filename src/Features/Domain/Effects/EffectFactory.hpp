#pragma once

#include <utility>

#include "Features/Domain/Effects/ActiveEffect.hpp"

namespace sw::features::domain::effects
{
    template<typename TData>
    ActiveEffect create(TData data, uint32_t ticks, uint32_t sourceUnitId, EffectApplyFn applyFn)
    {
        ActiveEffect effect;
        effect.data = std::move(data);
        effect.dataType = std::type_index(typeid(TData));
        effect.remainingTicks = ticks;
        effect.sourceUnitId = sourceUnitId;
        effect.applyFn = std::move(applyFn);
        return effect;
    }
}
