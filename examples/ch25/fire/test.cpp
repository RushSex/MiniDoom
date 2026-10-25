// test.cpp — под огнём импа: снаряды, урон, смерть и рестарт
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

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

void snapshot(doom::Game& g, const char* path) {
    doom::Canvas canvas(640, 400);
    g.render(canvas);
    doom::drawHud(canvas, g, 60);
    savePPM(canvas, path);
}
} // namespace

int main() {
    doom::Game game{doom::Map{doom::kLevel}};
    game.teleport(3.5, 11.5, 0.0);   // коридор, имп в 4 клетках
    bool shotSaved = false;
    int lastHp = game.health();
    for (int frame = 1; frame <= 1200 && !game.dead(); ++frame) {
        game.update({}, 1.0 / 60.0);
        if (!shotSaved && !game.shots().empty() &&
            game.shots().front().x < 5.0) {  // шар на подлёте
            snapshot(game, "frame.ppm");
            shotSaved = true;
        }
        if (game.health() != lastHp) {
            lastHp = game.health();
            std::cout << std::format("t {:5.2f}  hp {:3}  "
                                     "in flight {}\n",
                                     frame / 60.0, lastHp,
                                     game.shots().size());
        }
    }
    std::cout << std::format("dead: {}\n", game.dead());
    for (int frame = 0; frame < 90; ++frame) {
        game.update({}, 1.0 / 60.0);     // полторы секунды лежим
    }
    snapshot(game, "dead.ppm");
    game.update({.confirm = true}, 1.0 / 60.0);
    std::cout << std::format("after Enter: dead {}  hp {}  x {:.1f}"
                             "  monsters {}\n",
                             game.dead(), game.health(),
                             game.playerX(), game.monsters().size());
}
