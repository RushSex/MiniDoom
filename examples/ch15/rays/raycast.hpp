// raycast.hpp — луч по сетке карты: где он упрётся в стену
#pragma once

#include "map.hpp"

namespace doom {

struct RayHit {
    double dist = 0.0;   // в длинах вектора (dx, dy)
    int cellX = 0;       // клетка стены, в которую попали
    int cellY = 0;
    int side = 0;        // 0 — грань между столбцами, 1 — рядами
};

// Луч из точки (ox, oy) в направлении (dx, dy).
[[nodiscard]] RayHit castRay(const Map& map, double ox, double oy,
                             double dx, double dy);

} // namespace doom
