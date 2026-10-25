#include <array>
#include <cmath>
#include <format>
#include <iostream>
#include <string_view>

// Маленькая карта: # — стена.
constexpr std::array<std::string_view, 5> kMap{
    "######",
    "#....#",
    "#....#",
    "#...##",
    "######",
};

int main() {
    const double ox = 1.3, oy = 1.4;    // откуда
    const double dx = 0.8, dy = 0.6;    // куда, длина 1
    int cx = 1, cy = 1;                 // клетка, где стоим
    // Сколько пройти вдоль луча от одной линии сетки до следующей.
    const double stepX = 1.0 / dx;      // 1.25
    const double stepY = 1.0 / dy;      // 1.67
    double nextX = (cx + 1.0 - ox) * stepX;
    double nextY = (cy + 1.0 - oy) * stepY;
    for (int step = 1;; ++step) {
        std::cout << std::format("step {}: nextX {:.2f} "
                                 "nextY {:.2f}",
                                 step, nextX, nextY);
        double dist = 0.0;
        if (nextX < nextY) {
            dist = nextX;
            nextX += stepX;
            ++cx;
        } else {
            dist = nextY;
            nextY += stepY;
            ++cy;
        }
        const char c = kMap[static_cast<std::size_t>(cy)]
                           [static_cast<std::size_t>(cx)];
        std::cout << std::format(" -> ({},{}) '{}' at {:.2f}\n", cx,
                                 cy, c, dist);
        if (c == '#') {
            break;
        }
    }
}
