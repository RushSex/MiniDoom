// test.cpp — проверка текстур и кадр с ними
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "noise.hpp"
#include "textures.hpp"

#include <format>
#include <fstream>
#include <iostream>

namespace {
template <typename Picture>
void savePPM(const Picture& c, const char* path) {
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

// «Отпечаток» текстуры: средний цвет. Поменял генератор — он
// изменится, и тест это покажет.
std::uint32_t average(const doom::Image& img) {
    std::uint64_t r = 0, g = 0, b = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            const std::uint32_t p = img.at(x, y);
            r += (p >> 16) & 0xFF;
            g += (p >> 8) & 0xFF;
            b += p & 0xFF;
        }
    }
    const auto n = static_cast<std::uint64_t>(img.width() *
                                              img.height());
    return static_cast<std::uint32_t>((r / n) << 16 | (g / n) << 8 |
                                      b / n);
}
} // namespace

int main() {
    std::cout << std::format("hash01(5, 7) = {:.4f}, again {:.4f}\n",
                             doom::hash01(5, 7, 1),
                             doom::hash01(5, 7, 1));
    std::cout << std::format("noise at 0 and 4 (period 4): "
                             "{:.4f} {:.4f}\n",
                             doom::valueNoise(0.0, 0.5, 4, 1),
                             doom::valueNoise(4.0, 0.5, 4, 1));
    const char* names[] = {"stone", "bricks", "tech"};
    const char cells[] = {'1', '2', '3'};
    doom::Canvas sheet(3 * 64 + 2 * 8, 64);      // три в ряд
    sheet.clear(doom::rgb(255, 255, 255));
    for (int i = 0; i < 3; ++i) {
        const doom::Image& t = doom::wallTexture(cells[i]);
        std::cout << std::format("{:<7} {}x{}  average #{:06X}\n",
                                 names[i], t.width(), t.height(),
                                 average(t));
        for (int y = 0; y < 64; ++y) {
            for (int x = 0; x < 64; ++x) {
                sheet.put(i * 72 + x, y, t.at(x, y));
            }
        }
    }
    savePPM(sheet, "textures.ppm");

    doom::Game game{doom::Map{doom::kLevel}};
    game.update({.mouseDX = 160}, 1.0 / 60.0);     // ≈ 23° вправо
    for (int i = 0; i < 30; ++i) {
        game.update({.forward = true}, 1.0 / 60.0);
    }
    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
