// hud.hpp — интерфейс поверх мира: статус-бар, сообщения, FPS
#pragma once

#include "canvas.hpp"
#include "game.hpp"
#include "image.hpp"

namespace doom {

// Лицо героя 20 × 21: mood 0 бодр … 2 избит, 3 мёртв;
// look −1 / 0 / +1 — куда косится; ouch — только что попали.
[[nodiscard]] Image makeFace(int mood, int look, bool ouch);

void drawStatusBar(Canvas& canvas, const Game& game);
void drawHud(Canvas& canvas, const Game& game, int fps);

} // namespace doom
