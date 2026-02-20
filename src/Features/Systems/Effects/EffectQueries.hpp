#pragma once

#include "Core/World.hpp"

#include "Features/Domain/Effects/EffectList.hpp"

namespace sw::features::systems::effects
{
    template<typename TData>
    bool hasEffect(core::World& world, uint32_t unitId)
    {
        auto& effectMap = world.getComponent<domain::effects::EffectList>();
        auto listIt = effectMap.find(unitId);
        if (listIt == effectMap.end())
        {
            return false;
        }

        for (const auto& effect : listIt->second.active)
        {
            if (effect.dataType == std::type_index(typeid(TData)))
            {
                return true;
            }
        }

        return false;
    }
}
