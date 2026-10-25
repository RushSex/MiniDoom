// test.cpp — автотесты без окна: ctest запускает каждый отдельно
#include "app.hpp"
#include "levels.hpp"

#include <chrono>
#include <cmath>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <random>
#include <source_location>
#include <string_view>

namespace {
int g_failures = 0;

// Проверка, которая сообщает, где именно она не прошла.
void check(bool ok, std::string_view what,
           std::source_location at =
               std::source_location::current()) {
    if (!ok) {
        ++g_failures;
        std::cout << std::format("FAIL {}:{}: {}\n", at.file_name(),
                                 at.line(), what);
    }
}

void savePPM(const doom::Canvas& c, const std::string& path) {
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

// Всё ли в мире в порядке? Нарушение — баг, где бы он ни был.
void invariants(const doom::Game& g, int run, int frame) {
    const auto where = std::format("run {} frame {}", run, frame);
    check(g.health() >= 0 && g.health() <= 200, where + ": health");
    check(g.ammo() >= 0, where + ": ammo");
    const auto cx = static_cast<int>(std::floor(g.playerX()));
    const auto cy = static_cast<int>(std::floor(g.playerY()));
    check(!g.map().isWall(cx, cy), where + ": player in wall");
    for (const doom::Monster& m : g.monsters()) {
        const auto mx = static_cast<int>(std::floor(m.x));
        const auto my = static_cast<int>(std::floor(m.y));
        check(!g.map().isWall(mx, my), where + ": monster in wall");
    }
}

void testLevels() {
    const auto levels = doom::loadLevels(".");
    check(levels.size() >= 2, "at least two levels");
    std::cout << std::format("levels: {} loaded\n", levels.size());
}

// Робот-хаос: жмёт случайные клавиши и проверяет мир каждый кадр.
void testChaos() {
    constexpr int kRuns = 20;
    constexpr int kFrames = 60 * 60;            // минута на прогон
    std::mt19937 rng{2026};
    std::bernoulli_distribution coin(0.5), rare(0.02);
    std::uniform_int_distribution<int> mouse(-40, 40);
    int deaths = 0;
    int wins = 0;
    for (int run = 0; run < kRuns; ++run) {
        doom::App app(doom::loadLevels("."));
        app.warp(static_cast<std::size_t>(run % 2));  // оба уровня
        for (int frame = 0; frame < kFrames; ++frame) {
            const doom::Input in{
                .forward = coin(rng), .back = rare(rng),
                .strafeLeft = rare(rng), .strafeRight = rare(rng),
                .run = coin(rng), .fire = coin(rng),
                .weapon = rare(rng) ? 2 : 0, .use = rare(rng),
                .confirm = rare(rng), .mouseDX = mouse(rng)};
            app.update(in, 1.0 / 60.0);
            invariants(app.game(), run, frame);
            if (app.game().dead() && app.game().deadTime() < 1e-9) {
                ++deaths;
            }
            if (app.mode() == doom::Mode::Summary) {
                ++wins;
                break;
            }
        }
    }
    std::cout << std::format("chaos: {} runs, {} deaths, {} exits\n",
                             kRuns, deaths, wins);
}

// Автоскриншоты: кадры для глаз человека и простые проверки.
void testShots() {
    doom::App app(doom::loadLevels("."));
    doom::Canvas canvas(640, 400);
    const auto shoot = [&](std::string_view name) {
        app.render(canvas, 60);
        savePPM(canvas, std::format("shot_{}.ppm", name));
        // Над статус-баром должно быть что-то кроме фона: иначе
        // кадр просто не нарисовался.
        const int h = canvas.height() - doom::Game::kStatusBar;
        int busy = 0;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < canvas.width(); ++x) {
                busy += canvas.at(x, y) != canvas.at(0, 0);
            }
        }
        check(busy > 1000, std::format("{}: empty picture", name));
        std::cout << std::format("shot_{}.ppm: {} busy pixels\n",
                                 name, busy);
    };
    shoot("title");
    app.update({.confirm = true}, 1.0 / 60.0);
    shoot("game");
    app.update({.map = true}, 1.0 / 60.0);
    shoot("automap");
}

// Сколько миллисекунд занимает кадр: update + render.
void bench() {
    doom::App app(doom::loadLevels("."));
    app.warp(1);
    doom::Canvas canvas(640, 400);
    constexpr int kFrames = 300;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < kFrames; ++i) {
        app.update({.turnLeft = true}, 1.0 / 60.0);
        app.render(canvas, 60);
    }
    const std::chrono::duration<double, std::milli> spent =
        std::chrono::steady_clock::now() - start;
    std::cout << std::format("bench: {:.2f} ms per frame\n",
                             spent.count() / kFrames);
}
} // namespace

int main(int argc, char** argv) {
    const std::string_view what = argc > 1 ? argv[1] : "all";
    if (what == "levels" || what == "all") {
        testLevels();
    }
    if (what == "chaos" || what == "all") {
        testChaos();
    }
    if (what == "shots" || what == "all") {
        testShots();
    }
    if (what == "bench") {
        bench();
    }
    if (g_failures > 0) {
        std::cout << std::format("{} FAILED\n", g_failures);
        return 1;
    }
    std::cout << "OK\n";
}
