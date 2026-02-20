#pragma once

#include "Core/Commands/ICommand.hpp"
#include "Visitors/CommandParserVisitor.hpp"

#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace sw::core::io
{
	class CommandParser
	{
	public:
		template <class TCommand>
		void add()
		{
			std::string commandName = TCommand::Name;
			auto [it, inserted] = _commands.emplace(
					commandName,
					[](std::istream& stream) -> std::unique_ptr<commands::ICommand>
					{
						auto command = std::make_unique<TCommand>();
						CommandParserVisitor visitor(stream);
						command->visit(visitor);
						return command;
					});
			if (!inserted)
			{
				throw std::runtime_error("Command already exists: " + commandName);
			}
		}

		std::vector<std::unique_ptr<commands::ICommand>> parse(std::istream& stream) const;

	private:
		std::unordered_map<std::string, std::function<std::unique_ptr<commands::ICommand>(std::istream&)>> _commands;
	};
}
