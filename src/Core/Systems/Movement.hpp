#pragma once

#include "Core/World.hpp"
#include "Core/Systems/Spatial.hpp"
#include "Core/Events/UnitMoved.hpp"
#include "Core/Events/MarchEnded.hpp"

namespace sw::core::systems
{
    class Movement
    {
    public:
        static void processUnit(World& world, uint32_t unitId)
        {
            auto& targets = world.targetPositions;
            if (targets.find(unitId) == targets.end()) return;

            // 1. Check if some Feature-level system (like Battle) blocked us
            if (!world.restrictions.isAllowed(unitId, registry::restrictions::MOVE)) return;

            const Position targetPos = targets[unitId];
            const Position currentPos = world.positions[unitId];
            const Position nextPos = Spatial::getNextStep(currentPos, targetPos);
            if (Spatial::isPassable(world, nextPos))
            {
                world.positions[unitId] = nextPos;
                world.getEvents().event(world.getTick(), events::UnitMoved{unitId, nextPos.x, nextPos.y});

                if (nextPos == targetPos)
                {
                    world.getEvents().event(world.getTick(), events::MarchEnded{unitId, nextPos.x, nextPos.y});
                    targets.erase(unitId);
                }
            }
        }
    };
}