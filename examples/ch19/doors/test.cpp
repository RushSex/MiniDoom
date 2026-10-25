// test.cpp — робот открывает дверь и проверяет «не прищемить»
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

namespace {
void hold(doom::Game& game, const doom::Input& in, double seconds) {
    const int frames = static_cast<int>(seconds * 60.0 + 0.5);
    for (int i = 0; i < frames; ++i) {
        game.update(in, 1.0 / 60.0);
    }
}

void report(const doom::Game& g, std::string_view what) {
    const doom::Door* d = g.map().door(7, 5);   // дверь на пути
    std::cout << std::format("{:<16} x {:5.2f}  y {:5.2f}  "
                             "door {:.2f} {:+d}\n",
                             what, g.playerX(), g.playerY(), d->open,
                             d->move);
}

void savePPM(const doom::Canvas& c, const char* path) {
    std::ofstream f(path, std::ios::binary);
    f << "P6\n" << c.width() << ' ' << c.height() << "\n255\n";
    for (int y = 0; y < c.height(); ++y) {
        for (int x = 0; x < c.width(); ++x) {
            const std::uint32_t p = c.at(x, y);
            f.put(static_cast<char>((p >> 16) & 0xFF));
            f.put(static_cast<char>((p >> 8) & 0xFF));
            f.put(static_cast<char>(p & 0xFF));
        }
    }
}
} // namespace

int main() {
    doom::Game game{doom::Map{doom::kLevel}};
    hold(game, {.mouseDX = 628}, 1.0 / 60.0);       // на юг
    hold(game, {.forward = true}, 4.0 / 3.0);       // 4 клетки
    hold(game, {.mouseDX = -628}, 1.0 / 60.0);      // на восток
    hold(game, {.forward = true}, 1.4);
    report(game, "near the door");
    hold(game, {.use = true}, 1.0 / 60.0);
    hold(game, {}, 0.25);
    report(game, "E + 0.25 s");

    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");

    hold(game, {}, 0.25);
    report(game, "E + 0.5 s");
    hold(game, {.forward = true}, 1.5);
    report(game, "walked through");
    hold(game, {}, 1.25);
    report(game, "waited 1.25 s");
    hold(game, {}, 0.5);
    report(game, "waited 0.5 s");
    hold(game, {}, 1.0);
    report(game, "waited 1 s");

    hold(game, {.mouseDX = 1257}, 1.0 / 60.0);      // разворот
    hold(game, {.forward = true}, 1.0);             // к двери
    hold(game, {.use = true}, 1.0 / 60.0);
    hold(game, {}, 0.5);
    hold(game, {.forward = true}, 0.25);            // встаём в проём
    hold(game, {}, 6.0);
    report(game, "in the doorway");
    hold(game, {.forward = true}, 1.0);             // уходим
    hold(game, {}, 4.0);
    report(game, "left, waited 4 s");
}
