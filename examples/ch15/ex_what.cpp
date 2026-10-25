#include <cmath>
#include <format>
#include <iostream>

int main() {
    const double ox = 2.25, oy = 0.4;   // откуда
    const double dx = -0.6, dy = 0.8;   // куда: влево и вниз
    const int cx = static_cast<int>(std::floor(ox));
    const int cy = static_cast<int>(std::floor(oy));
    const double stepX = std::abs(1.0 / dx);
    const double stepY = std::abs(1.0 / dy);
    const double nextX = dx < 0.0 ? (ox - cx) * stepX
                                  : (cx + 1.0 - ox) * stepX;
    const double nextY = dy < 0.0 ? (oy - cy) * stepY
                                  : (cy + 1.0 - oy) * stepY;
    std::cout << std::format("{:.2f} {:.2f} -> {}\n", nextX, nextY,
                             nextX < nextY ? "x" : "y");
}
