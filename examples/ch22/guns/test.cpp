// test.cpp — тир: стреляем по импу и зомби в коридоре
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

#include <format>
#include <fstream>
#include <iostream>

namespace {
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

// Здоровье монстра номер i (как он записан в векторе).
int hp(const doom::Game& g, std::size_t i) {
    return g.monsters()[i].hp;
}
} // namespace

int main() {
    doom::Game game{doom::Map{doom::kLevel}};
    // Монстры в векторе по порядку карты: 0 — зомби вверху,
    // 1 — имп и 2 — зомби в коридоре, 3 — имп в нижнем зале.
    game.teleport(3.5, 11.5, 0.0);           // коридор, на восток
    doom::Canvas canvas(640, 400);
    int shots = 0;
    int last = game.ammo();
    // Держим огонь две с половиной секунды, кадр — 1/60.
    for (int frame = 0; frame < 150; ++frame) {
        game.update({.fire = true}, 1.0 / 60.0);
        if (game.ammo() != last) {           // этот кадр — выстрел
            last = game.ammo();
            ++shots;
            std::cout << std::format("shot {}: imp {:3}  zombie {:3}"
                                     "  ammo {}\n",
                                     shots, hp(game, 1), hp(game, 2),
                                     last);
            if (shots == 2) {                // кадр с попаданием
                game.render(canvas);
                doom::drawHud(canvas, game, 60);
                savePPM(canvas, "frame.ppm");
            }
        }
    }
    game.update({.weapon = 2}, 0.5);         // дробовик, пауза
    game.update({.fire = true}, 1.0 / 60.0);
    std::cout << std::format("shotgun: zombie {}  ammo {}  kills {}"
                             "\n",
                             hp(game, 2), game.ammo(), game.kills());
}
