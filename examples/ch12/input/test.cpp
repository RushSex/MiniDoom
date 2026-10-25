// test.cpp — робот играет без окна: ввод по сценарию, проверки
#include "game.hpp"

#include <format>
#include <iostream>

namespace {
// Держать ввод in заданное число секунд при 60 кадрах в секунду.
void hold(doom::Game& game, doom::Input in, double seconds) {
    const int frames = static_cast<int>(seconds * 60.0);
    for (int i = 0; i < frames; ++i) {
        game.update(in, 1.0 / 60.0);
        in.fire = false;            // щелчок — только в первом кадре
        in.mouseDX = 0;
        in.mouseDY = 0;
    }
}

void report(const char* what, const doom::Game& g) {
    std::cout << std::format("{:<16} player ({:5.1f},{:5.1f})  "
                             "aim ({:5.1f},{:5.1f})  shots {}\n",
                             what, g.playerX(), g.playerY(),
                             g.aimX(), g.aimY(), g.shots());
}
} // namespace

int main() {
    doom::Game game(640, 400);
    report("start", game);
    hold(game, {.right = true}, 0.5);
    report("right 0.5 s", game);
    hold(game, {.right = true, .run = true}, 0.5);
    report("run 0.5 s", game);
    hold(game, {.up = true, .left = true}, 1.0);
    report("up+left 1 s", game);
    hold(game, {.right = true}, 10.0);
    report("right 10 s", game);
    hold(game, {.fire = true, .mouseDX = -200, .mouseDY = 60}, 0.1);
    report("mouse + click", game);
}
