#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

namespace sw::core::registry
{
    // Standard restriction tags used across Core systems
    namespace restrictions
    {
        const std::string MOVE = "move";     // Completely blocks movement
        const std::string ATTACK = "attack"; // Completely blocks attacks
    }

    class RestrictionsRegistry
    {
    public:
        bool isAllowed(uint32_t unitId, const std::string& tag) const
        {
            auto unitIt = data.find(unitId);
            if (unitIt == data.end()) return true;

            auto tagIt = unitIt->second.find(tag);
            return (tagIt == unitIt->second.end()) || (tagIt->second <= 0);
        }

        void modify(uint32_t unitId, const std::string& tag, int8_t delta)
        {
            data[unitId][tag] += delta;
        }

        void clear() { data.clear(); }

    private:
        std::unordered_map<uint32_t, std::unordered_map<std::string, int8_t>> data;
    };
}