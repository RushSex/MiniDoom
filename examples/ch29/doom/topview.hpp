// topview.hpp — автокарта: только то, что игрок уже видел
#pragma once

#include "canvas.hpp"
#include "game.hpp"

namespace doom {

void drawTopView(Canvas& canvas, const Game& game);

} // namespace doom
