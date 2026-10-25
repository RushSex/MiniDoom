// hud.cpp — интерфейс поверх картинки: здоровье, патроны, сообщения
#include "hud.hpp"

#include "text.hpp"

#include <format>

namespace doom {

void drawHud(Canvas& canvas, const Game& game, int fps) {
    const int w = canvas.width();
    const int h = canvas.height();
    const auto fpsText = std::format("{} FPS", fps);
    const int fpsX = w - 8 - textWidth(fpsText, 2);
    drawShadowText(canvas, fpsX, 8, fpsText, rgb(90, 255, 90), 2);

    if (!game.message().empty()) {      // как в Doom: слева вверху
        drawShadowText(canvas, 8, 8, game.message(),
                       rgb(255, 216, 160), 2);
    }
    // Здоровье краснеет, когда его мало.
    const auto hpColor = game.health() > 30 ? rgb(230, 230, 230)
                                            : rgb(255, 60, 40);
    const auto hp = std::format("{}%", game.health());
    drawShadowText(canvas, 8, h - 30, hp, hpColor, 3);
    const auto ammo = std::format("AMMO {}", game.ammo());
    drawShadowText(canvas, w - 8 - textWidth(ammo, 3), h - 30, ammo,
                   rgb(255, 210, 60), 3);
}

} // namespace doom
