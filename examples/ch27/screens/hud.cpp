// hud.cpp — статус-бар как в Doom и надписи поверх картинки
#include "hud.hpp"

#include "art.hpp"
#include "paint.hpp"
#include "text.hpp"

#include <array>
#include <format>
#include <string_view>
#include <vector>

namespace doom {

namespace {
void dot(Image& img, int x, int y, int w, int h, std::uint32_t c) {
    for (int j = y; j < y + h; ++j) {
        for (int i = x; i < x + w; ++i) {
            img.set(i, j, opaque(c));
        }
    }
}

// Подпись мелко, значение крупно — одна ячейка статус-бара.
void field(Canvas& c, int x, int w, std::string_view label,
           std::string_view value, std::uint32_t color) {
    const int y0 = c.height() - Game::kStatusBar;
    const int vx = x + (w - textWidth(value, 3)) / 2;
    drawShadowText(c, vx, y0 + 7, value, color, 3);
    const int lx = x + (w - textWidth(label, 1)) / 2;
    drawText(c, lx, y0 + 33, label, rgb(170, 160, 140), 1);
}
} // namespace

Image makeFace(int mood, int look, bool ouch) {
    Image img(20, 21);
    const auto skin =
        mood == 3 ? rgb(150, 110, 90) : rgb(200, 140, 100);
    fillEllipse(img, {10, 11}, 8, 9.5, skin);             // голова
    fillEllipse(img, {10, 4}, 8, 3.5, rgb(90, 60, 30));   // волосы
    const int ex = 6 + look;
    if (mood == 3) {                                      // глаза X
        for (int i = 0; i < 3; ++i) {
            dot(img, ex + i, 9 + i, 1, 1, rgb(40, 0, 0));
            dot(img, ex + 2 - i, 9 + i, 1, 1, rgb(40, 0, 0));
            dot(img, ex + 7 + i, 9 + i, 1, 1, rgb(40, 0, 0));
            dot(img, ex + 9 - i, 9 + i, 1, 1, rgb(40, 0, 0));
        }
    } else {
        dot(img, ex, 10, 3, 2, rgb(240, 240, 240));       // белки
        dot(img, ex + 7, 10, 3, 2, rgb(240, 240, 240));
        dot(img, ex + 1 + look, 10, 1, 2, rgb(30, 30, 60)); // зрачки
        dot(img, ex + 8 + look, 10, 1, 2, rgb(30, 30, 60));
        const int frown = mood == 0 ? 0 : 1;              // брови
        dot(img, ex, 8 + frown, 3, 1, rgb(70, 40, 20));
        dot(img, ex + 7, 8 + frown, 3, 1, rgb(70, 40, 20));
    }
    if (ouch && mood != 3) {
        dot(img, 8, 15, 4, 3, rgb(60, 10, 10));           // «а-а!»
    } else {
        const int len = 6 - 2 * (mood % 3);              // рот
        dot(img, 7 + mood, 16, len, 1, rgb(90, 30, 20));
    }
    if (mood >= 2) {                                      // кровь
        dot(img, 3, 12, 2, 4, rgb(170, 20, 20));
        dot(img, 14, 5, 2, 3, rgb(170, 20, 20));
    }
    return img;
}

void drawStatusBar(Canvas& canvas, const Game& game) {
    const int w = canvas.width();
    const int y0 = canvas.height() - Game::kStatusBar;
    canvas.fillRect(0, y0, w, Game::kStatusBar, rgb(64, 60, 54));
    canvas.fillRect(0, y0, w, 2, rgb(130, 122, 108));     // фаска
    for (const int x : {110, 230, 296, 344, 470}) {       // ячейки
        canvas.fillRect(x, y0 + 4, 2, Game::kStatusBar - 8,
                        rgb(40, 36, 32));
    }
    const auto red = rgb(220, 40, 30);
    field(canvas, 0, 110, "AMMO", std::format("{}", game.ammo()),
          red);
    field(canvas, 110, 120, "HEALTH",
          std::format("{}%", game.health()), red);
    const bool pistol = game.weapon() == Weapon::Pistol;
    const auto on = rgb(255, 220, 60);         // оружие в руках
    const auto off = rgb(120, 112, 100);
    drawShadowText(canvas, 244, y0 + 9, "1", pistol ? on : off, 2);
    drawShadowText(canvas, 272, y0 + 9, "2", pistol ? off : on, 2);
    drawText(canvas, 249, y0 + 33, "ARMS", rgb(170, 160, 140), 1);
    field(canvas, 344, 126, "KILLS",
          std::format("{}/{}", game.kills(), game.monstersTotal()),
          red);
    const auto secs = static_cast<int>(game.time());
    field(canvas, 470, 170, "TIME",
          std::format("{}:{:02}", secs / 60, secs % 60), red);

    // Лицо: 4 настроения × 3 взгляда × «больно», строим один раз.
    static const auto faces = [] {
        std::vector<Image> f;
        for (int i = 0; i < 24; ++i) {
            f.push_back(makeFace(i / 6, i / 2 % 3 - 1, i % 2 == 1));
        }
        return f;
    }();
    const int mood = game.dead()           ? 3
                     : game.health() > 66 ? 0
                     : game.health() > 33 ? 1
                                          : 2;
    // Косится по кругу: прямо, влево, прямо, вправо — раз в 1,5 с.
    constexpr std::array<int, 4> kLook{0, -1, 0, 1};
    const auto phase =
        static_cast<std::size_t>(game.time() / 1.5) % 4;
    const int look = kLook[phase] + 1;          // 0, 1, 2
    const bool ouch = game.hurt() > 0.3;
    const auto index = static_cast<std::size_t>(mood * 6 + look * 2 +
                                                (ouch ? 1 : 0));
    const Image& face = faces[index];
    for (int y = 0; y < face.height(); ++y) {
        for (int x = 0; x < face.width(); ++x) {
            const std::uint32_t c = face.at(x, y);
            if ((c >> 24) != 0) {
                canvas.fillRect(300 + x * 2, y0 + 2 + y * 2, 2, 2,
                                c & 0xFFFFFF);
            }
        }
    }
}

void drawHud(Canvas& canvas, const Game& game, int fps) {
    const int w = canvas.width();
    const auto fpsText = std::format("{} FPS", fps);
    const int fpsX = w - 8 - textWidth(fpsText, 2);
    drawShadowText(canvas, fpsX, 8, fpsText, rgb(90, 255, 90), 2);
    if (!game.message().empty()) {      // как в Doom: слева вверху
        drawShadowText(canvas, 8, 8, game.message(),
                       rgb(255, 216, 160), 2);
    }
    if (game.dead()) {                  // крупно и по центру
        const std::string_view died = "YOU DIED";
        const int dx = (w - textWidth(died, 7)) / 2;
        drawShadowText(canvas, dx, 120, died, rgb(255, 255, 255), 7);
        if (game.deadTime() > 1.0) {
            const std::string_view again = "PRESS ENTER";
            const int ax = (w - textWidth(again, 3)) / 2;
            const auto pink = rgb(255, 210, 200);
            drawShadowText(canvas, ax, 200, again, pink, 3);
        }
    }
    drawStatusBar(canvas, game);
}

} // namespace doom
