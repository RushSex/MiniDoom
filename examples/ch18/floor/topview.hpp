// topview.hpp — карта сверху: будущая автокарта
#pragma once

#include "canvas.hpp"
#include "map.hpp"

namespace doom {

void drawTopView(Canvas& canvas, const Map& map, double x, double y,
                 double angle);

} // namespace doom
