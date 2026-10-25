// raycast.cpp — алгоритм DDA: шагаем от линии сетки к линии
#include "raycast.hpp"

#include <cmath>

namespace doom {

namespace {
constexpr double kFar = 1e30;   // «бесконечность» для луча вдоль оси
constexpr int kMaxSteps = 256;  // страховка от вечного цикла
} // namespace

RayHit castRay(const Map& map, double ox, double oy, double dx,
               double dy) {
    int cx = static_cast<int>(std::floor(ox));   // текущая клетка
    int cy = static_cast<int>(std::floor(oy));
    // Сколько длин луча между соседними линиями сетки по x и по y.
    const double stepX = dx == 0.0 ? kFar : std::abs(1.0 / dx);
    const double stepY = dy == 0.0 ? kFar : std::abs(1.0 / dy);
    const int moveX = dx < 0.0 ? -1 : 1;
    const int moveY = dy < 0.0 ? -1 : 1;
    // Сколько длин луча до первой линии сетки по каждой оси.
    double nextX = dx < 0.0 ? (ox - cx) * stepX
                            : (cx + 1.0 - ox) * stepX;
    double nextY = dy < 0.0 ? (oy - cy) * stepY
                            : (cy + 1.0 - oy) * stepY;

    for (int i = 0; i < kMaxSteps; ++i) {
        RayHit hit;
        if (nextX < nextY) {        // ближе линия между столбцами
            hit.dist = nextX;
            nextX += stepX;
            cx += moveX;
            hit.side = 0;
        } else {                    // ближе линия между рядами
            hit.dist = nextY;
            nextY += stepY;
            cy += moveY;
            hit.side = 1;
        }
        if (map.isWall(cx, cy)) {
            hit.cellX = cx;
            hit.cellY = cy;
            return hit;
        }
    }
    return {kFar, cx, cy, 0};
}

} // namespace doom
