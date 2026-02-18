#pragma once

#include "Core/World.hpp"

#include <vector>

namespace sw::game
{
    class Spatial
    {
    public:
        static bool isPassable(const World& world, Position pos)
        {
            if (pos.x >= world.map.width || pos.y >= world.map.height) return false;
            
            for (const auto& [id, unitPos] : world.positions)
            {
                if (unitPos == pos) return false;
            }

            return true;
        }

        static void findTargets(const World& world, uint32_t selfId, Position center, std::vector<uint32_t>& out)
        {
            for (const auto& [id, pos] : world.positions)
            {
                if (id == selfId) continue;
                if (std::abs((int)pos.x - (int)center.x) <= 1 && 
                    std::abs((int)pos.y - (int)center.y) <= 1)
                {
                    out.push_back(id);
                }
            }
        }
    };
}