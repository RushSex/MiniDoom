// test.cpp — подбираем предметы; кадр со спрайтами
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <string_view>

namespace {
void hold(doom::Game& game, const doom::Input& in, double seconds) {
    const int frames = static_cast<int>(seconds * 60.0 + 0.5);
    for (int i = 0; i < frames; ++i) {
        game.update(in, 1.0 / 60.0);
    }
}

void report(const doom::Game& g, std::string_view what) {
    const auto left = std::ranges::count(g.items(), false,
                                         &doom::Item::taken);
    std::cout << std::format("{:<14} x {:4.2f}  hp {:3}  ammo {:2}  "
                             "left {}  \"{}\"\n",
                             what, g.playerX(), g.health(), g.ammo(),
                             left, g.message());
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

    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");

    hold(game, {.forward = true}, 0.9);
    report(game, "forward 0.9 s");
    hold(game, {.forward = true}, 0.7);
    report(game, "forward 0.7 s");
    hold(game, {}, 2.0);
    report(game, "waited 2 s");
}
