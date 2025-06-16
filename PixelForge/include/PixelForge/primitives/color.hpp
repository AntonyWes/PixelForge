#pragma once

#include <cstdint>

namespace PixelForge {

class Color {
    public:
        Color() = default;
        Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
        explicit Color(uint32_t argb);

        uint32_t toARGB32() const;
        static uint16_t toRGB565(uint32_t argb);
        uint32_t toXRGB8888() const;

        uint8_t r() const;
        uint8_t g() const;
        uint8_t b() const;
        uint8_t a() const;

        static Color fromRGB565(uint16_t color);
        static Color blend(const Color& src, const Color& dst);

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Black;
        static const Color White;
        static const Color Transparent;

    private:
        uint8_t r_ = 0;
        uint8_t g_ = 0;
        uint8_t b_ = 0;
        uint8_t a_ = 255;
};

}
