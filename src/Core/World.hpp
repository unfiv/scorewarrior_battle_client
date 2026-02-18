#pragma once

#include "Core/Position.hpp"
#include "Core/Map.hpp"

#include <unordered_map>

namespace sw::core::io
{
    class EventSystem;
}

namespace sw::core
{
    class World
    {
    public:
        explicit World(io::EventSystem& events) : eventSystem(eventSystem) {}

        uint32_t getTick() const { return tick; }
        void nextTick() { tick++; }
        io::EventSystem& getEvents() { return eventSystem; }

        Map map{0, 0};

        std::unordered_map<uint32_t, Position> positions;

        bool isGameOver() const { return tick > 100; }

    private:
        uint32_t tick{0};
        io::EventSystem& eventSystem;
    };
}