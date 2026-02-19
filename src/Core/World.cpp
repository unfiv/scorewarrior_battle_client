#include "World.hpp"

#include <algorithm>

namespace sw::core
{
	void World::nextTick()
	{
		++tick;

		restrictions.clear();

        // Create a stable snapshot of unit IDs to ensure predictable processing order
        std::vector<uint32_t> ids;
        for (auto const& [id, _] : positions) ids.push_back(id);
        std::sort(ids.begin(), ids.end());

        for (size_t i = 0; i < systems.size(); ++i)
        {
            if (std::holds_alternative<GlobalSystem>(systems[i]))
            {
                // Execute world-level logic that doesn't depend on individual units
                std::get<GlobalSystem>(systems[i])(*this);
            }
            else
            {
                // We found a block of unit-level systems. 
                // Group consecutive UnitSystems to process them in a "turn-based" manner.
                size_t j = i;
                while (j < systems.size() && std::holds_alternative<UnitSystem>(systems[j]))
                {
                    j++;
                }

                // For each unit, execute the entire sequence of grouped systems 
                // before moving to the next unit. This ensures that if Unit 1 moves, 
                // Unit 2 will see its new position within the same tick's Battle phase.
                for (uint32_t id : ids)
                {
                    for (size_t k = i; k < j; ++k)
                    {
                        // If a previous system in the sequence destroyed the unit, stop processing it
                        if (positions.find(id) == positions.end())
                        {
                            break;
                        }
                        std::get<UnitSystem>(systems[k])(*this, id);
                    }
                }
                
                // Advance the outer loop index to the end of the processed block
                i = j - 1;
            }
        }        
	}
}
