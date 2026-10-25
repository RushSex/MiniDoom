#include <format>
#include <iostream>

int main() {
    const double dx = 1.0, dy = 0.0;    // стреляем на восток
    const double radius = 0.3;
    // Монстр далеко слева от линии выстрела.
    const double rx = 5.0, ry = 3.0;
    const double along = rx * dx + ry * dy;
    const double off = rx * dy - ry * dx;
    const bool hit = along > 0.0 && off < radius;
    std::cout << std::format("off {:.1f} -> {}\n", off,
                             hit ? "HIT" : "miss");
}
