// hud.cpp — интерфейс поверх картинки
#include "hud.hpp"

#include "text.hpp"

#include <format>

namespace doom {

void drawHud(Canvas& canvas, const Game& game, int fps) {
    const int w = canvas.width();
    const int h = canvas.height();
    drawShadowText(canvas, 8, 8, "MINI DOOM", rgb(230, 40, 20), 3);

    const auto fpsText = std::format("{} FPS", fps);
    const int fpsX = w - 8 - textWidth(fpsText, 2);    // по правому
    drawShadowText(canvas, fpsX, 8, fpsText, rgb(90, 255, 90), 2);

    const auto pos = std::format("X {:3}  Y {:3}",
                                 static_cast<int>(game.playerX()),
                                 static_cast<int>(game.playerY()));
    drawShadowText(canvas, 8, h - 22, pos, rgb(220, 220, 220), 2);

    const auto shots = std::format("SHOTS {}", game.shots());
    const int shotsX = w - 8 - textWidth(shots, 2);
    drawShadowText(canvas, shotsX, h - 22, shots,
                   rgb(255, 210, 60), 2);
}

} // namespace doom
