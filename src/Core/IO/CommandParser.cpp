#include "CommandParser.hpp"

namespace sw::core::io
{
	std::vector<std::unique_ptr<commands::ICommand>> CommandParser::parse(std::istream& stream) const
	{
		std::vector<std::unique_ptr<commands::ICommand>> result;

		std::string line;
		while (std::getline(stream, line))
		{
			if (line.rfind("//", 0) == 0 || line.empty())
			{
				continue;
			}

			std::istringstream commandStream(line);
			std::string commandName;
			commandStream >> commandName;

			if (commandName.empty())
			{
				continue;
			}

			auto command = _commands.find(commandName);
			if (command == _commands.end())
			{
				throw std::runtime_error("Unknown command: " + commandName);
			}

			result.push_back(command->second(commandStream));
		}

		return result;
	}
}
