// test.cpp — поле расстояний и имп, который идёт на выстрел
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "pathfind.hpp"

#include <array>
#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

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

std::string_view name(doom::MonsterState s) {
    constexpr std::array<std::string_view, 5> names{
        "Idle", "Chase", "Attack", "Pain", "Dead"};
    return names[static_cast<std::size_t>(s)];
}

// Поле расстояний в файл, строками: стена — #, дверь — D.
void saveField(const doom::Game& g, const char* path) {
    std::ofstream f(path);
    const doom::Map& map = g.map();
    for (int y = 0; y < map.height(); ++y) {
        for (int x = 0; x < map.width(); ++x) {
            const auto i =
                static_cast<std::size_t>(y * map.width() + x);
            if (map.isWall(x, y)) {
                f << " #";
            } else if (map.door(x, y) != nullptr) {
                f << " D";
            } else {
                f << ' ' << g.flow()[i];
            }
        }
        f << '\n';
    }
}
} // namespace

int main() {
    doom::Game game{doom::Map{doom::kLevel}};
    game.teleport(2.5, 8.5, 0.0);            // комната над залом
    game.update({}, 1.0 / 60.0);             // посчитать поле
    saveField(game, "field.txt");

    const auto& imp = game.monsters()[1];    // имп в коридоре зала
    std::cout << std::format("before the shot: imp {}\n",
                             name(imp.state));
    game.update({.fire = true}, 1.0 / 60.0); // бах!
    for (int step = 1; step <= 13; ++step) {
        for (int f = 0; f < 30; ++f) {
            game.update({}, 1.0 / 60.0);
        }
        const doom::Door* door = game.map().door(7, 10);
        std::cout << std::format("t {:3.1f}  {:<6} x {:4.2f}  "
                                 "y {:5.2f}  door {:.2f}  hp {}\n",
                                 step * 0.5, name(imp.state), imp.x,
                                 imp.y, door->open, game.health());
    }
    doom::Canvas canvas(640, 400);
    game.teleport(2.5, 8.5, -1.08);          // повернуться к импу
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
