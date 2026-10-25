// test.cpp — высоты столбцов и кадр из глаз игрока
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "raycast.hpp"

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
} // namespace

int main() {
    const doom::Map map{doom::kLevel};
    // Стоим на старте (1.5, 1.5) и смотрим на восток.
    for (int col : {0, 160, 320, 480, 639}) {
        const double cam = 2.0 * (col + 0.5) / 640 - 1.0;
        const auto hit =
            doom::castRay(map, 1.5, 1.5, 1.0, 0.8 * cam);
        std::cout << std::format("col {:3}  cell ({},{})  side {}  "
                                 "dist {:.2f}  height {:3}\n",
                                 col, hit.cellX, hit.cellY, hit.side,
                                 hit.dist,
                                 static_cast<int>(400 / hit.dist));
    }
    doom::Game game{map};
    for (int i = 0; i < 15; ++i) {      // четверть секунды вперёд
        game.update({.forward = true}, 1.0 / 60.0);
    }
    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
