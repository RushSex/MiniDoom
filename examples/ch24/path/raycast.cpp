// raycast.cpp — DDA; стены и двери посреди клетки
#include "raycast.hpp"

#include <cmath>

namespace doom {

namespace {
constexpr double kFar = 1e30;   // «бесконечность» для луча вдоль оси
constexpr int kMaxSteps = 256;  // страховка от вечного цикла
} // namespace

RayHit castRay(const Map& map, double ox, double oy, double dx,
               double dy) {
    int cx = static_cast<int>(std::floor(ox));
    int cy = static_cast<int>(std::floor(oy));
    const double stepX = dx == 0.0 ? kFar : std::abs(1.0 / dx);
    const double stepY = dy == 0.0 ? kFar : std::abs(1.0 / dy);
    const int moveX = dx < 0.0 ? -1 : 1;
    const int moveY = dy < 0.0 ? -1 : 1;
    double nextX = dx < 0.0 ? (ox - cx) * stepX
                            : (cx + 1.0 - ox) * stepX;
    double nextY = dy < 0.0 ? (oy - cy) * stepY
                            : (cy + 1.0 - oy) * stepY;
    // Где на грани точка, до которой луч прошёл dist.
    const auto along = [&](int side, double dist) {
        const double v = side == 0 ? oy + dy * dist : ox + dx * dist;
        return v - std::floor(v);
    };

    for (int i = 0; i < kMaxSteps; ++i) {
        RayHit hit;
        if (nextX < nextY) {
            hit.dist = nextX;
            nextX += stepX;
            cx += moveX;
            hit.side = 0;
        } else {
            hit.dist = nextY;
            nextY += stepY;
            cy += moveY;
            hit.side = 1;
        }
        hit.cellX = cx;
        hit.cellY = cy;
        if (const Door* door = map.door(cx, cy)) {
            // Плита на середине клетки: ещё полшага вдоль луча.
            const double mid =
                hit.dist + (hit.side == 0 ? stepX : stepY) / 2;
            const double exit = hit.side == 0 ? nextY : nextX;
            if (mid > exit) {
                continue;       // луч ушёл из клетки вбок раньше
            }
            const double u = along(hit.side, mid);
            if (u < door->open) {
                continue;       // попали в щель уехавшей двери
            }
            hit.dist = mid;
            hit.u = u - door->open;    // текстура уезжает с дверью
            return hit;
        }
        if (map.isWall(cx, cy)) {
            hit.u = along(hit.side, hit.dist);
            // На «обратных» гранях переворачиваем: не зеркалить.
            if ((hit.side == 0 && dx < 0.0) ||
                (hit.side == 1 && dy > 0.0)) {
                hit.u = 1.0 - hit.u;
            }
            return hit;
        }
    }
    return {kFar, cx, cy, 0, 0.0};
}

} // namespace doom
