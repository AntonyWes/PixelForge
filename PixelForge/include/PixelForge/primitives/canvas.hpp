#pragma once

#include <vector>
#include <utility>
#include "../core/fb_context.hpp"
#include "color.hpp"

namespace PixelForge {

class Canvas {
    public:
        explicit Canvas(FBContext& fb);

        void clear(uint32_t color);
        void drawPixel(int x, int y, uint32_t color);

        void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
        void drawRect(int x, int y, int w, int h, uint32_t color);
        void fillRect(int x, int y, int w, int h, uint32_t color);
        void fillRoundedRect(int x, int y, int w, int h, int radius, uint32_t color);
        void fillGradientRect(int x, int y, int w, int h, uint32_t topColor, uint32_t bottomColor);
        void drawCircle(int x, int y, int radius, uint32_t color);
        void fillCircle(int x0, int y0, int radius, uint32_t color);
        void drawPolygon(const std::vector<std::pair<int, int>> points, uint32_t color);
        void fillPolygon(const std::vector<std::pair<int, int>> points, uint32_t color) {scanlineFill(points, color);}

        void setBlendMode(bool enable);
        uint32_t width() const;
        uint32_t height() const;

    private:
        FBContext& fb_;
        bool blend_enabled_ = false;

        void plotCirclePoints(int xc, int yc, int x, int y, uint32_t color);
        void scanlineFill(const std::vector<std::pair<int, int>> points, uint32_t color);
};

}
