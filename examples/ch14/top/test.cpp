// test.cpp — робот ходит по карте без окна
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

#include <format>
#include <fstream>
#include <iostream>
#include <numbers>
#include <string_view>

namespace {
// Держим ввод seconds секунд при 60 кадрах в секунду.
void hold(doom::Game& game, const doom::Input& in, double seconds) {
    const int frames = static_cast<int>(seconds * 60.0 + 0.5);
    for (int i = 0; i < frames; ++i) {
        game.update(in, 1.0 / 60.0);
    }
}

void report(const doom::Game& g, std::string_view what) {
    const double deg = g.angle() * 180.0 / std::numbers::pi;
    std::cout << std::format("{:<18} x {:5.2f}  y {:5.2f}  {:4.0f}"
                             " deg\n",
                             what, g.playerX(), g.playerY(), deg);
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
    report(game, "start");
    hold(game, {.forward = true}, 1.0);
    report(game, "forward 1 s");
    hold(game, {.forward = true}, 2.0);
    report(game, "forward 2 s more");
    hold(game, {.mouseDX = 314}, 1.0 / 60.0);
    report(game, "mouse +314");
    hold(game, {.forward = true}, 1.0);
    report(game, "forward 1 s");
    hold(game, {.strafeLeft = true}, 1.0);
    report(game, "strafe left 1 s");
    hold(game, {.turnRight = true}, 0.5);
    report(game, "turn right 0.5 s");
    hold(game, {.forward = true, .run = true}, 1.0);
    report(game, "run 1 s");

    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
