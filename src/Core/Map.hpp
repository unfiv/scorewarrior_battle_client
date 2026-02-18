#pragma once

#include <cstdint>

namespace sw::core
{
    struct Map
    {
        uint32_t width{0};
        uint32_t height{0};
        
        bool isValid() const { return width > 0 && height > 0; }
    };
}