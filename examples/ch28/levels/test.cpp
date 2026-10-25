// test.cpp — уровни: загрузка, ошибки в файлах, переход дальше
#include "app.hpp"
#include "levels.hpp"
#include "pathfind.hpp"

#include <algorithm>
#include <format>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

namespace {
// Сводка по уровню: размер, население и путь до выхода.
void describe(const doom::Level& lv) {
    const auto count = [&](std::string_view what) {
        long n = 0;
        for (const std::string& row : lv.rows) {
            n += std::ranges::count_if(row, [&](char c) {
                return what.find(c) != std::string_view::npos;
            });
        }
        return n;
    };
    const doom::Map map = lv.map();
    const auto dist = doom::distanceField(map, *map.find('P'));
    const doom::Cell x = *map.find('X');
    std::cout << std::format(
        "{:<8} {}x{}  monsters {}  items {}  exit {} steps\n",
        lv.name, map.width(), map.height(), count("iz"), count("ha"),
        dist[static_cast<std::size_t>(x.y * map.width() + x.x)]);
}

void tryParse(std::string_view text) {
    std::istringstream in{std::string(text)};
    try {
        const doom::Level lv = doom::parseLevel(in, "test");
        std::cout << "ok: " << lv.name << '\n';
    } catch (const doom::LevelError& e) {
        std::cout << e.what() << '\n';
    }
}
} // namespace

int main(int argc, char** argv) {
    if (argc == 3 && std::string_view(argv[1]) == "check") {
        try {                           // проверка своего уровня
            describe(doom::loadLevel(argv[2]));
        } catch (const doom::LevelError& e) {
            std::cout << "error: " << e.what() << '\n';
            return 1;
        }
        return 0;
    }
    auto levels = doom::loadLevels(".");
    for (const doom::Level& lv : levels) {
        describe(lv);
    }
    tryParse("1111\n1PX1\n1111\n");
    tryParse("name A\n11111\n1P.X1\n1111\n");
    tryParse("name B\n11111\n1PqX1\n11111\n");
    tryParse("name C\n11111\n1P.X.\n11111\n");
    tryParse("name D\n11111\n1P1X1\n11111\n");

    doom::App app(std::move(levels));
    app.update({.confirm = true}, 1.0 / 60.0);       // Enter
    doom::Game& g = app.game();
    for (int i = 0; i < 60; ++i) {                   // стреляем 1 с
        app.update({.fire = true}, 1.0 / 60.0);
    }
    g.teleport(13.5, 13.5, 0.0);                     // у выхода
    for (int i = 0; i < 30; ++i) {
        app.update({.forward = true}, 1.0 / 60.0);
    }
    std::cout << std::format("{}: {}, ammo {}\n", app.level().name,
                             doom::modeName(app.mode()), g.ammo());
    app.update({.confirm = true}, 1.0 / 60.0);       // досчитать
    app.update({.confirm = true}, 1.0 / 60.0);       // дальше
    const doom::Loadout l = app.game().loadout();
    std::cout << std::format("{}: {}, health {}, ammo {}\n",
                             app.level().name,
                             doom::modeName(app.mode()), l.health,
                             l.ammo);
}
