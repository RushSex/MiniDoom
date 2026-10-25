// hud.cpp — интерфейс поверх картинки
#include "hud.hpp"

#include "text.hpp"

#include <cmath>
#include <format>
#include <numbers>

namespace doom {

void drawHud(Canvas& canvas, const Game& game, int fps) {
    const int w = canvas.width();
    const int h = canvas.height();
    drawShadowText(canvas, 8, 8, "MINI DOOM", rgb(230, 40, 20), 2);

    const auto fpsText = std::format("{} FPS", fps);
    const int fpsX = w - 8 - textWidth(fpsText, 2);
    drawShadowText(canvas, fpsX, 8, fpsText, rgb(90, 255, 90), 2);

    // Округляем и прибавляем 0.0: так −0.3° не превратится в «−0».
    const double deg =
        std::round(game.angle() * 180.0 / std::numbers::pi) + 0.0;
    const auto pos = std::format("X {:4.1f}  Y {:4.1f}  A {:4.0f}",
                                 game.playerX(), game.playerY(),
                                 deg);
    drawShadowText(canvas, 8, h - 22, pos, rgb(220, 220, 220), 2);
}

} // namespace doom
