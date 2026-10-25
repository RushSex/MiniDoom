// test.cpp — тот же кадр, что в walls, но с рыбьим глазом
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

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
    doom::Game game{doom::Map{doom::kLevel}};
    for (int i = 0; i < 15; ++i) {
        game.update({.forward = true}, 1.0 / 60.0);
    }
    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
    std::cout << "frame.ppm saved\n";
}
