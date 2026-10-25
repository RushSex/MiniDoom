// canvas.cpp — рисование в память; никакой Windows
#include "canvas.hpp"

#include <algorithm>

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

std::uint32_t Canvas::at(int x, int y) const {
    return pixels_[index(x, y)];
}

} // namespace doom
