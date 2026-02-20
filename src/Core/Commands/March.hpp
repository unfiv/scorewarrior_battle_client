#pragma once

#include <cstdint>

#include "Core/Commands/ICommand.hpp"
#include "Core/IO/EventSystem.hpp"
#include "Core/Events/MarchStarted.hpp"

namespace sw::core::commands
{
	struct March : public ICommand
	{
		constexpr static const char* Name = "MARCH";

		uint32_t unitId{};
		uint32_t targetX{};
		uint32_t targetY{};

		void execute(sw::core::World& world) const override
		{
			if (auto it = world.positions.find(unitId); it != world.positions.end())
			{
				const Position currentPos = it->second;
				const Position targetPos = {targetX, targetY};
				world.targetPositions[unitId] = targetPos;
				world.getEvents().event(world.getTick(), events::MarchStarted{unitId, currentPos.x, currentPos.y, targetPos.x, targetPos.y});
			}
		}

		template <typename Visitor>
		void visit(Visitor& visitor)
		{
			visitor.visit("unitId", unitId);
			visitor.visit("targetX", targetX);
			visitor.visit("targetY", targetY);
		}
	};
}
