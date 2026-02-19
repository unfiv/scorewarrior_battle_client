#include "World.hpp"

namespace sw::core
{
	void World::nextTick()
	{
		++tick;

		restrictions.clear();

		for (auto& system : systems)
		{
            system(*this);
        }
	}
}
