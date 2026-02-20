#pragma once

#include <cstdint>

#include "Core/Commands/ICommand.hpp"
#include "Core/IO/EventSystem.hpp"
#include "Core/Events/MapCreated.hpp"

namespace sw::core::commands
{
	struct CreateMap : public ICommand
	{
		constexpr static const char* Name = "CREATE_MAP";

		uint32_t width{};
		uint32_t height{};

		void execute(sw::core::World& world) const override
		{
			world.map = {width, height};
			world.getEvents().event(world.getTick(), events::MapCreated{width, height});
		}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("width", width);
			visitor.visit("height", height);
		}
	};
}
