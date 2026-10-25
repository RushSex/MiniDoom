// test.cpp — лучи из стартовой клетки: проверяем руками
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "raycast.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

namespace {
void ray(const doom::Map& map, std::string_view name, double dx,
         double dy) {
    const doom::RayHit h = doom::castRay(map, 1.5, 1.5, dx, dy);
    std::cout << std::format("{:<10} cell ({:2},{:2})  side {}  "
                             "dist {:.3f}\n",
                             name, h.cellX, h.cellY, h.side, h.dist);
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
    const doom::Map map{doom::kLevel};
    ray(map, "east", 1.0, 0.0);
    ray(map, "south", 0.0, 1.0);
    ray(map, "west", -1.0, 0.0);
    ray(map, "south-east", 0.7071, 0.7071);
    ray(map, "east x2", 2.0, 0.0);      // вектор длиной 2

    doom::Game game{map};
    game.update({.mouseDX = 314}, 1.0 / 60.0);   // 45°
    for (int i = 0; i < 20; ++i) {
        game.update({.forward = true}, 1.0 / 60.0);
    }
    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
