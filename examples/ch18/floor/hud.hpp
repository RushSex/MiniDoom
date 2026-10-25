// hud.hpp — надписи поверх мира: название, FPS, координаты
#pragma once

#include "canvas.hpp"
#include "game.hpp"

namespace doom {

void drawHud(Canvas& canvas, const Game& game, int fps);

} // namespace doom
