#pragma once

#include <cstdint>

namespace sw::core
{
    struct Position
    {
        uint32_t x{0};
        uint32_t y{0};

        // This tells the compiler to generate comparison operators (==, !=)
        bool operator==(const Position&) const = default;
    };
}