#pragma once

#include "Core/World.hpp"
#include <cmath>
#include <algorithm>

namespace sw::core::systems
{
    class Spatial
    {
    public:
        static bool isPassable(const World& world, Position pos)
        {
            if (pos.x >= world.map.width || pos.y >= world.map.height)
            {
                return false;
            }

            // Collision check: verify if any unit already occupies this coordinate
            for (const auto& [id, unitPos] : world.positions)
            {
                if (unitPos == pos)
                {
                    return false;
                }
            }

            return true;
        }

        static Position getNextStep(Position current, Position target)
        {
            Position next = current;

            // Step in X direction
            if (target.x < current.x)      next.x -= 1;
            else if (target.x > current.x) next.x += 1;

            // Step in Y direction
            if (target.y < current.y)      next.y -= 1;
            else if (target.y > current.y) next.y += 1;

            return next;
        }

        static void findTargets(const World& world, uint32_t selfId, Position center, std::vector<uint32_t>& out)
        {
            for (const auto& [id, pos] : world.positions)
            {
                if (id == selfId) continue;
                
                // Check if unit is in the adjacent 8 tiles or the same tile
                if (std::abs(static_cast<int>(pos.x) - static_cast<int>(center.x)) <= 1 && 
                    std::abs(static_cast<int>(pos.y) - static_cast<int>(center.y)) <= 1)
                {
                    out.push_back(id);
                }
            }
        }
    };
}