#include "../../include/PixelForge/primitives/color.hpp"

namespace PixelForge {

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
    : r_(r), g_(g), b_(b), a_(a) {}

Color::Color(uint32_t argb)
    : a_((argb >> 24) & 0xFF),
      r_((argb >> 16) & 0xFF),
      g_((argb >> 8) & 0xFF),
      b_(argb & 0xFF) {}

uint32_t Color::toARGB32() const {
    return (a_ << 24) | (r_ << 16) | (g_ << 8) | b_;
}

uint16_t Color::toRGB565(uint32_t argb) {
    uint8_t r = (argb >> 16) & 0xFF;
    uint8_t g = (argb >> 8) & 0xFF;
    uint8_t b = argb & 0xFF;
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

uint32_t Color::toXRGB8888() const {
    return (0xFF << 24) | (r_ << 16) | (g_ << 8) | b_;
}

uint8_t Color::r() const { return r_; }
uint8_t Color::g() const { return g_; }
uint8_t Color::b() const { return b_; }
uint8_t Color::a() const { return a_; }

Color Color::fromRGB565(uint16_t color) {
    return Color(
        ((color >> 11) & 0x1F) << 3,
        ((color >> 5) & 0x3F) << 2,
        (color & 0x1F) << 3
    );
}

Color Color::blend(const Color& src, const Color& dst) {
    if (src.a() == 255) return src;
    if (src.a() == 0) return dst;

    float alpha = src.a() / 255.0f;
    float inv_alpha = 1.0f - alpha;

    return Color(
        static_cast<uint8_t>(src.r() * alpha + dst.r() * inv_alpha),
        static_cast<uint8_t>(src.g() * alpha + dst.g() * inv_alpha),
        static_cast<uint8_t>(src.b() * alpha + dst.b() * inv_alpha),
        static_cast<uint8_t>(src.a() + dst.a() * inv_alpha)
    );
}
const Color Color::Red(255, 0, 0);
const Color Color::Green(0, 255, 0);
const Color Color::Blue(0, 0, 255);
const Color Color::Black(0, 0, 0);
const Color Color::White(255, 255, 255);
const Color Color::Transparent(0, 0, 0, 0);

}
