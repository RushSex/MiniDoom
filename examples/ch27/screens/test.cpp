// test.cpp — проходим все экраны и снимаем каждый на память
#include "app.hpp"
#include "hud.hpp"

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

doom::App app;
double clock = 0.0;

// Прожить seconds секунд. in — нажатие в первом кадре или,
// если hold, зажатые клавиши на всё время.
void live(double seconds, doom::Input in = {}, bool hold = false) {
    const doom::Mode before = app.mode();
    for (int i = 0; i < static_cast<int>(seconds * 60); ++i) {
        app.update(i == 0 || hold ? in : doom::Input{}, 1.0 / 60.0);
        clock += 1.0 / 60.0;
    }
    if (app.mode() != before) {
        std::cout << std::format("t {:5.2f}  {:<8} -> {}\n", clock,
                                 doom::modeName(before),
                                 doom::modeName(app.mode()));
    }
}

void shot(const char* path) {
    doom::Canvas canvas(640, 400);
    app.render(canvas, 60);
    savePPM(canvas, path);
}
} // namespace

int main() {
    live(1.0);
    shot("title.ppm");
    live(0.5, {.confirm = true});                  // Enter
    live(1.2, {.forward = true}, true);
    live(0.5, {.map = true});                      // Tab: автокарта
    shot("automap.ppm");
    live(0.1, {.map = true});
    live(0.4, {.pause = true});                    // Esc
    shot("paused.ppm");
    live(0.5, {.pause = true});
    doom::Game& g = app.game();
    g.teleport(13.5, 13.5, 0.0);                   // рядом с выходом
    live(0.5, {.forward = true}, true);            // шаг на выход
    const auto secs = static_cast<int>(g.time());
    std::cout << std::format("kills {}/{}  items {}/{}  "
                             "time {}:{:02}\n",
                             g.kills(), g.monstersTotal(),
                             g.itemsTaken(), g.itemsTotal(),
                             secs / 60, secs % 60);
    live(0.6);
    shot("counting.ppm");
    live(1.5);
    shot("summary.ppm");
    live(0.1, {.confirm = true});
    live(3.0);
    shot("credits.ppm");
    live(0.1, {.confirm = true});
    // Все 24 лица героя одной картинкой: ряд — настроение.
    doom::Canvas sheet(6 * 22, 4 * 23);
    sheet.clear(doom::rgb(64, 60, 54));
    for (int i = 0; i < 24; ++i) {
        const doom::Image f =
            doom::makeFace(i / 6, i / 2 % 3 - 1, i % 2 == 1);
        for (int y = 0; y < f.height(); ++y) {
            for (int x = 0; x < f.width(); ++x) {
                if ((f.at(x, y) >> 24) != 0) {
                    sheet.put(i % 6 * 22 + 1 + x, i / 6 * 23 + 1 + y,
                              f.at(x, y) & 0xFFFFFF);
                }
            }
        }
    }
    savePPM(sheet, "faces.ppm");
}
