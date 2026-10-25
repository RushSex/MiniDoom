// test.cpp — монстры замечают игрока, идут к нему и атакуют
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"

#include <array>
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

std::string_view name(doom::MonsterState s) {
    constexpr std::array<std::string_view, 5> names{
        "Idle", "Chase", "Attack", "Pain", "Dead"};
    return names[static_cast<std::size_t>(s)];
}

// Стоим на месте seconds секунд и раз в step секунд печатаем,
// что делает монстр номер i.
void watch(doom::Game& g, std::size_t i, double seconds,
           double step) {
    const int frames = static_cast<int>(step * 60.0 + 0.5);
    for (double t = step; t <= seconds + 1e-9; t += step) {
        for (int f = 0; f < frames; ++f) {
            g.update({}, 1.0 / 60.0);
        }
        const doom::Monster& m = g.monsters()[i];
        std::cout << std::format("  t {:4.1f}  {:<6} x {:5.2f}  "
                                 "y {:5.2f}  hp {:3}\n",
                                 t, name(m.state), m.x, m.y,
                                 g.health());
    }
}
} // namespace

int main() {
    doom::Game game{doom::Map{doom::kLevel}};
    // Монстр 0 — зомби во второй комнате, в (12.5, 4.5).
    std::cout << "behind the pillar:\n";
    game.teleport(10.5, 2.5, 0.0);          // колонна 3 между нами
    watch(game, 0, 1.0, 1.0);
    std::cout << "in the open:\n";
    game.teleport(9.5, 4.5, 0.0);           // ряд 4: видно насквозь
    watch(game, 0, 4.0, 0.5);

    // Монстр 1 — имп в коридоре нижнего зала, в (7.5, 11.5).
    doom::Game hall{doom::Map{doom::kLevel}};
    hall.teleport(3.5, 11.5, 0.0);
    std::cout << "imp in the corridor:\n";
    watch(hall, 1, 3.0, 0.5);

    doom::Canvas canvas(640, 400);
    hall.render(canvas);
    doom::drawHud(canvas, hall, 60);
    savePPM(canvas, "frame.ppm");
}
