// test.cpp — проверки шрифта при компиляции и кадр с HUD
#include "font.hpp"
#include "game.hpp"
#include "hud.hpp"
#include "text.hpp"

#include <format>
#include <fstream>
#include <iostream>

// Эти проверки выполняет компилятор: ошибёшься — не соберётся.
static_assert(doom::glyphFor('a') == doom::glyphFor('A'));
static_assert(doom::glyphFor('~') == doom::glyphFor(' '));
static_assert(doom::glyphFor('1')[6] == 0x0E);   // подставка «1»

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
    std::cout << std::format("width of \"MINI DOOM\" x3: {} px\n",
                             doom::textWidth("MINI DOOM", 3));
    std::cout << std::format("width of \"60 FPS\" x2:    {} px\n",
                             doom::textWidth("60 FPS", 2));
    doom::Canvas canvas(640, 400);
    doom::Game game(640, 400);
    game.update({.right = true, .fire = true, .mouseDX = 80}, 0.5);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
    std::cout << "frame.ppm saved\n";
}
