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
        static void update(World& world)
        {
            auto& targets = world.targetPositions;
            auto& positions = world.positions;

            for (auto it = targets.begin(); it != targets.end(); )
            {
                const uint32_t unitId = it->first;

                if (!world.restrictions.isAllowed(unitId, registry::restrictions::MOVE))
                {
                    ++it;
                    continue;
                }

                const Position targetPos = it->second;
                const Position currentPos = positions[unitId];

                Position nextPos = Spatial::getNextStep(currentPos, targetPos);
                if (Spatial::isPassable(world, nextPos))
                {
                    positions[unitId] = nextPos;
                    
                    world.getEvents().event(world.getTick(), events::UnitMoved{unitId, nextPos.x, nextPos.y});

                    if (nextPos == targetPos)
                    {
                        world.getEvents().event(world.getTick(), events::MarchEnded{unitId, nextPos.x, nextPos.y});
                        it = targets.erase(it); 
                        continue;
                    }
                }
                else
                {
                    // Logic for path blocking (e.g., waiting or re-pathing) can be added here
                }
                ++it;
            }
        }
    };
}