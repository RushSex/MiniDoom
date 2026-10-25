// image.hpp — картинка в памяти: текстура или спрайт.
// Цвет 0xAARRGGBB: у спрайтов альфа 0 — прозрачный пиксель.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace doom {

class Image {
public:
    Image(int width, int height)
        : width_(width), height_(height),
          pixels_(static_cast<std::size_t>(width * height)) {}

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] std::uint32_t at(int x, int y) const {
        return pixels_[index(x, y)];
    }
    void set(int x, int y, std::uint32_t color) {
        pixels_[index(x, y)] = color;
    }

private:
    [[nodiscard]] std::size_t index(int x, int y) const {
        return static_cast<std::size_t>(y * width_ + x);
    }

    int width_;
    int height_;
    std::vector<std::uint32_t> pixels_;
};

} // namespace doom
