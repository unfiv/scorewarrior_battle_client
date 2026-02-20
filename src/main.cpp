#include "Core/World.hpp"

#include "Core/CommandProcessor.hpp"
#include "Core/IO/EventSystem.hpp"
#include "Core/Systems/Movement.hpp"

#include "Features/Systems/Death.hpp"
#include "Features/Systems/Effects.hpp"
#include "Features/Systems/MeleeAttack.hpp"
#include "Features/Systems/RangedAttack.hpp"

#include <fstream>

int main(int argc, char** argv)
{
	using namespace sw::core;
	using namespace sw::core::io;

	if (argc != 2)
	{
		throw std::runtime_error("Error: No file specified in command line argument");
	}

	std::ifstream file(argv[1]);
	if (!file)
	{
		throw std::runtime_error("Error: File not found - " + std::string(argv[1]));
	}

	EventSystem events;
	World world(events);
	CommandProcessor commandProcessor(world);

	world.systems.push_back(sw::features::systems::Effects::processUnit);
	world.systems.push_back(sw::features::systems::Death::processUnit);
	world.systems.push_back(sw::features::systems::RangedAttack::processUnit);
	world.systems.push_back(sw::features::systems::MeleeAttack::processUnit);
	world.systems.push_back(sw::core::systems::Movement::processUnit);
	world.systems.push_back(sw::features::systems::Death::update);

	commandProcessor.process(file);

	while (!world.isGameOver())
	{
		world.nextTick();
	}

	return 0;
}
