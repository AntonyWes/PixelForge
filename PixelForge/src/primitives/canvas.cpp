#include "../../include/PixelForge/primitives/canvas.hpp"
#include "../../include/PixelForge/primitives/color.hpp"
#include <algorithm>
#include <cmath>

namespace PixelForge {
Canvas::Canvas(FBContext& fb) : fb_(fb){}
void Canvas::clear(uint32_t color) {
    uint8_t* buffer = fb_.getBuffer();
    const size_t buffer_size = fb_.pitch() * fb_.height();

    switch (fb_.bpp()) {
        case 16: {
            uint16_t color16 = Color::toRGB565(color);
            uint16_t* ptr = reinterpret_cast<uint16_t*>(buffer);
            std::fill(ptr, ptr + (buffer_size / 2), color16);
            break;
        }
        case 24: {
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            for (size_t i = 0; i < buffer_size; i += 3) {
                buffer[i] = r;
                buffer[i+1] = g;
                buffer[i+2] = b;
            }
            break;
        }
        case 32: {
            uint32_t* ptr = reinterpret_cast<uint32_t*>(buffer);
            std::fill(ptr, ptr + (buffer_size / 4), color);
            break;
        }
    }
}

void Canvas::drawPixel(int x, int y, uint32_t color) {
    if (x < 0 || x >= fb_.width() || y < 0 || y >= fb_.height()) return;

    uint8_t* pixel = fb_.getBuffer() + y * fb_.pitch() + x * (fb_.bpp() / 8);

    switch (fb_.bpp()) {
        case 16:
            *reinterpret_cast<uint16_t*>(pixel) = Color::toRGB565(color);
            break;
        case 24:
            pixel[0] = (color >> 16) & 0xFF; // R
            pixel[1] = (color >> 8) & 0xFF;  // G
            pixel[2] = color & 0xFF;         // B
            break;
        case 32:
            *reinterpret_cast<uint32_t*>(pixel) = color;
            break;
    }
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }
}

void Canvas::fillCircle(int x0, int y0, int r, uint32_t color) {
    int x = r;
    int y = 0;
    int err = 0;

    while (x >= y) {
        drawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
        drawLine(x0 - y, y0 + x, x0 + y, y0 + x, color);
        drawLine(x0 - x, y0 - y, x0 + x, y0 - y, color);
        drawLine(x0 - y, y0 - x, x0 + y, y0 - x, color);

        if (err <= 0) {
            y += 1;
            err += 2*y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2*x + 1;
        }
    }
}

void Canvas::fillRect(int x, int y, int w, int h, uint32_t color) {
    if (x >= fb_.width() || y >= fb_.height()) return;
    if (x + w < 0 || y + h < 0) return;

    int x1 = std::max(x, 0);
    int y1 = std::max(y, 0);
    int x2 = std::min(x + w, static_cast<int>(fb_.width()));
    int y2 = std::min(y + h, static_cast<int>(fb_.height()));

    switch (fb_.bpp()) {
        case 16: {
            uint16_t color16 = Color::toRGB565(color);
            for (int row = y1; row < y2; ++row) {
                uint16_t* ptr = reinterpret_cast<uint16_t*>(
                    fb_.getBuffer() + row * fb_.pitch() + x1 * 2
                );
                std::fill(ptr, ptr + (x2 - x1), color16);
            }
            break;
        }
        case 24: {
            uint8_t r = (color >> 16) & 0xFF;
            uint8_t g = (color >> 8) & 0xFF;
            uint8_t b = color & 0xFF;
            for (int row = y1; row < y2; ++row) {
                uint8_t* ptr = fb_.getBuffer() + row * fb_.pitch() + x1 * 3;
                for (int col = x1; col < x2; ++col) {
                    *ptr++ = r;
                    *ptr++ = g;
                    *ptr++ = b;
                }
            }
            break;
        }
        case 32: {
            for (int row = y1; row < y2; ++row) {
                uint32_t* ptr = reinterpret_cast<uint32_t*>(
                    fb_.getBuffer() + row * fb_.pitch() + x1 * 4
                );
                std::fill(ptr, ptr + (x2 - x1), color);
            }
            break;
        }
    }
}
void Canvas::drawPolygon(const std::vector<std::pair<int, int>> points, uint32_t color) {
    if (points.size() < 2) return;

    for (size_t i = 0; i < points.size(); ++i) {
        size_t j = (i + 1) % points.size();
        drawLine(
            points[i].first, points[i].second,
            points[j].first, points[j].second,
            color
        );
    }
}

void Canvas::scanlineFill(const std::vector<std::pair<int, int>> points, uint32_t color) {
    if (points.size() < 3) return;

    int min_y = points[0].second;
    int max_y = points[0].second;
    for (const auto& p : points) {
        min_y = std::min(min_y, p.second);
        max_y = std::max(max_y, p.second);
    }

    for (int y = min_y; y <= max_y; ++y) {
        std::vector<int> intersections;

        for (size_t i = 0; i < points.size(); ++i) {
            size_t j = (i + 1) % points.size();
            int x1 = points[i].first;
            int y1 = points[i].second;
            int x2 = points[j].first;
            int y2 = points[j].second;

            if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
                int x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                intersections.push_back(x);
            }
        }

        std::sort(intersections.begin(), intersections.end());

        for (size_t i = 0; i < intersections.size(); i += 2) {
            int x1 = std::max(0, intersections[i]);
            int x2 = std::min(static_cast<int>(fb_.width()),
                             (i + 1 < intersections.size()) ? intersections[i + 1] : intersections[i]);
            fillRect(x1, y, x2 - x1, 1, color);
        }
    }
}

}
