#include <cmath>
#include <format>
#include <iostream>

int main() {
    const double dx = 0.0, dy = 1.0;    // стреляем на юг
    const double rx = 0.25, ry = 4.0;   // монстр относительно нас
    const double along = rx * dx + ry * dy;
    const double off = std::abs(rx * dy - ry * dx);
    std::cout << std::format("{:.2f} {:.2f} {}\n", along, off,
                             off < 0.3 ? "hit" : "miss");
}
