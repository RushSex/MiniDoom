// test.cpp — кадр со старта: спрайты без проверки Z-буфера
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

#include <algorithm>
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
} // namespace

int main() {
    doom::Game game{doom::Map{doom::kLevel}};
    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
    std::cout << "frame.ppm saved\n";
}
