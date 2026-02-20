#pragma once

#include "Core/World.hpp"
#include "Features/Domain/Effects/EffectList.hpp"

namespace sw::features::systems
{
    class Effects
    {
    public:
        static void processUnit(core::World& world, uint32_t targetId)
        {
            auto& effectMap = world.getComponent<domain::effects::EffectList>();
            
            // Take effect list for the current target, if it exists
            auto itList = effectMap.find(targetId);
            if (itList == effectMap.end()) return;

            auto& effectList = itList->second;
            auto it = effectList.active.begin();
            
            while (it != effectList.active.end())
            {
                if (it->applyFn)
                {
                    it->applyFn(world, targetId, *it);
                }

                if (it->remainingTicks > 0)
                {
                    --it->remainingTicks;
                }

                if (it->remainingTicks == 0)
                {
                    it = effectList.active.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }

        static void addEffect(core::World& world, uint32_t targetId, domain::effects::ActiveEffect effect)
        {
            world.getComponent<domain::effects::EffectList>()[targetId].active.push_back(std::move(effect));
        }
    };
}