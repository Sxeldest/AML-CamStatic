#pragma once

#include <cstdint>
#include "RW/RenderWare.h"

class CRGBA
{
public:
    uint8_t r{}, g{}, b{}, a{};

public:
    CRGBA() = default;
    CRGBA(uint8_t red, uint8_t green, uint8_t blue) : r(red), g(green), b(blue), a(255) {}
    CRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) : r(red), g(green), b(blue), a(alpha) {}

    void Set(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) {
        r = red; g = green; b = blue; a = alpha;
    }
};
