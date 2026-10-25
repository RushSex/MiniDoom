// canvas.hpp — холст: прямоугольник пикселей в памяти
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace doom {

// Цвет 0x00RRGGBB: по байту на красный, зелёный и синий.
[[nodiscard]] constexpr std::uint32_t rgb(std::uint32_t r,
                                          std::uint32_t g,
                                          std::uint32_t b) {
    return (r << 16) | (g << 8) | b;
}

class Canvas {
public:
    Canvas(int width, int height);

    void clear(std::uint32_t color);
    void put(int x, int y, std::uint32_t color);
    void fillRect(int x, int y, int w, int h, std::uint32_t color);
    void line(int x0, int y0, int x1, int y1, std::uint32_t color);

    [[nodiscard]] int width() const { return width_; }
    [[nodiscard]] int height() const { return height_; }
    [[nodiscard]] std::uint32_t at(int x, int y) const;
    [[nodiscard]] const std::uint32_t* data() const {
        return pixels_.data();
    }

private:
    [[nodiscard]] std::size_t index(int x, int y) const;

    int width_;
    int height_;
    std::vector<std::uint32_t> pixels_;
};

} // namespace doom
