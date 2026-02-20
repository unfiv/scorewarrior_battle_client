#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <any>
#include <variant>

#include "Core/Position.hpp"
#include "Core/Map.hpp"
#include "Core/Registry/RestrictionsRegistry.hpp"

namespace sw::core::io
{
    class EventSystem;
}

namespace sw::core
{
    class World;

    using UnitSystem = std::function<void(World&, uint32_t)>;
    using GlobalSystem = std::function<void(World&)>;
    using System = std::variant<UnitSystem, GlobalSystem>;

    class World
    {
    public:
        explicit World(io::EventSystem& events) : eventSystem(events) {}

        uint32_t getTick() const { return tick; }
        void nextTick();
        io::EventSystem& getEvents() { return eventSystem; }

        Map map{0, 0};

        std::vector<System> systems;

        std::vector<uint32_t> creationOrder;
        std::unordered_map<uint32_t, Position> positions;
        std::unordered_map<uint32_t, Position> targetPositions;
        
        registry::RestrictionsRegistry restrictions;

        template<typename T>
        std::unordered_map<uint32_t, T>& getComponent()
        {
            auto& storage = components[std::type_index(typeid(T))];
            if (!storage.has_value())
            {
                storage = std::make_any<std::unordered_map<uint32_t, T>>();
            }
            return std::any_cast<std::unordered_map<uint32_t, T>&>(storage);
        }

        bool isGameOver() const { return tick > 100; }

    private:
        uint32_t tick{0};
        io::EventSystem& eventSystem;
        std::unordered_map<std::type_index, std::any> components;
    };
}