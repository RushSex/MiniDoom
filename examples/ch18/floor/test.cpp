// test.cpp — свет, расстояния до пола, кадр; с аргументом — замер
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "shade.hpp"

#include <chrono>
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

// Сколько миллисекунд в среднем уходит на один кадр.
double msPerFrame(doom::Game& game, doom::Canvas& canvas, int n) {
    using Clock = std::chrono::steady_clock;
    const auto start = Clock::now();
    for (int i = 0; i < n; ++i) {
        game.update({.mouseDX = 3}, 1.0 / 60.0);  // поворот
        game.render(canvas);
    }
    const std::chrono::duration<double, std::milli> ms =
        Clock::now() - start;
    return ms.count() / n;
}
} // namespace

int main(int argc, char* argv[]) {
    doom::Game game{doom::Map{doom::kLevel}};
    doom::Canvas canvas(640, 400);
    if (argc > 1 && std::string_view(argv[1]) == "bench") {
        const double ms = msPerFrame(game, canvas, 500);
        std::cout << std::format("{:.2f} ms per frame, {:.0f} FPS\n",
                                 ms, 1000.0 / ms);
        return 0;
    }
    std::cout << std::format("shade(FF8040, 128) = {:06X}\n",
                             doom::shade(0xFF8040, 128));
    for (double d : {0.0, 2.0, 4.0, 8.0}) {
        std::cout << std::format("light at {:.0f}: {:3}\n", d,
                                 doom::lightAt(d));
    }
    for (int row : {201, 250, 300, 399}) {
        std::cout << std::format("row {}: floor at {:.2f}\n", row,
                                 200.0 / (row - 200.0 + 0.5));
    }
    for (int i = 0; i < 15; ++i) {          // смотрим вдоль коридора
        game.update({.forward = true}, 1.0 / 60.0);
    }
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
