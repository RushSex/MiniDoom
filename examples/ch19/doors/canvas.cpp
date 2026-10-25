// canvas.cpp — рисование в память; никакой Windows
#include "canvas.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>

namespace doom {

Canvas::Canvas(int width, int height)
    : width_(width), height_(height),
      pixels_(static_cast<std::size_t>(width) *
              static_cast<std::size_t>(height)) {}

std::size_t Canvas::index(int x, int y) const {
    const auto row = static_cast<std::size_t>(y);
    const auto col = static_cast<std::size_t>(x);
    return row * static_cast<std::size_t>(width_) + col;
}

void Canvas::clear(std::uint32_t color) {
    std::ranges::fill(pixels_, color);
}

void Canvas::put(int x, int y, std::uint32_t color) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        return;                       // за краем — молча пропускаем
    }
    pixels_[index(x, y)] = color;
}

void Canvas::fillRect(int x, int y, int w, int h,
                      std::uint32_t color) {
    // Обрезаем прямоугольник по краям холста один раз,
    // чтобы внутри цикла не проверять каждый пиксель.
    const int x0 = std::max(x, 0);
    const int y0 = std::max(y, 0);
    const int x1 = std::min(x + w, width_);
    const int y1 = std::min(y + h, height_);
    for (int py = y0; py < y1; ++py) {
        for (int px = x0; px < x1; ++px) {
            pixels_[index(px, py)] = color;
        }
    }
}

void Canvas::line(int x0, int y0, int x1, int y1,
                  std::uint32_t color) {
    // Шагов столько, сколько пикселей по длинной оси.
    const int steps = std::max(std::abs(x1 - x0), std::abs(y1 - y0));
    for (int i = 0; i <= steps; ++i) {
        const double t =
            steps == 0 ? 0.0 : static_cast<double>(i) / steps;
        const auto x = std::lround(x0 + (x1 - x0) * t);
        const auto y = std::lround(y0 + (y1 - y0) * t);
        put(static_cast<int>(x), static_cast<int>(y), color);
    }
}

std::uint32_t Canvas::at(int x, int y) const {
    return pixels_[index(x, y)];
}

} // namespace doom
