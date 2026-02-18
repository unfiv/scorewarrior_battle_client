#include "World.hpp"

namespace sw::core
{
	void World::nextTick()
	{
		++tick;

		for (auto& system : systems)
		{
            system(*this);
        }
	}
}
