// view3d.hpp — что видит игрок: стены в перспективе
#pragma once

#include "canvas.hpp"
#include "map.hpp"

namespace doom {

void drawView(Canvas& canvas, const Map& map, double x, double y,
              double angle);

} // namespace doom
