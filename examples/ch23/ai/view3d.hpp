// view3d.hpp — что видит игрок: стены в перспективе
#pragma once

#include "canvas.hpp"
#include "map.hpp"

#include <span>

namespace doom {

inline constexpr double kPlane = 0.8;   // полуширина окна камеры

// Рисует пол, потолок и стены. В zbuf записывает расстояние
// до стены в каждом столбце — оно понадобится спрайтам.
void drawView(Canvas& canvas, const Map& map, double x, double y,
              double angle, std::span<double> zbuf);

} // namespace doom
