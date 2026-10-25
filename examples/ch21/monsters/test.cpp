// test.cpp — кадры монстров отдельно и монстры на уровне
#include "game.hpp"
#include "hud.hpp"
#include "level.hpp"
#include "monsters.hpp"

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

// Сколько непрозрачных пикселей в кадре — «вес» рисунка.
int solid(const doom::Image& img) {
    int n = 0;
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            n += (img.at(x, y) >> 24) != 0 ? 1 : 0;
        }
    }
    return n;
}
} // namespace

int main() {
    using doom::MonsterKind;
    using doom::Pose;
    doom::Canvas sheet(4 * 64 + 3 * 8, 64);        // четыре кадра
    sheet.clear(doom::rgb(255, 255, 255));
    int slot = 0;
    for (MonsterKind k : {MonsterKind::Imp, MonsterKind::Zombie}) {
        for (Pose p : {Pose::Walk1, Pose::Walk2}) {
            const doom::Image& img = doom::monsterImage(k, p);
            std::cout << std::format("frame {}: {} solid pixels\n",
                                     slot, solid(img));
            for (int y = 0; y < 64; ++y) {
                for (int x = 0; x < 64; ++x) {
                    const auto c = img.at(x, y);
                    if ((c >> 24) != 0) {
                        sheet.put(slot * 72 + x, y, c & 0xFFFFFF);
                    }
                }
            }
            ++slot;
        }
    }
    savePPM(sheet, "monsters.ppm");

    doom::Game game{doom::Map{doom::kLevel}};
    std::cout << std::format("monsters on level: {}\n",
                             game.monsters().size());
    game.teleport(3.5, 11.5, 0.1);       // нижний зал, на восток
    doom::Canvas canvas(640, 400);
    game.render(canvas);
    doom::drawHud(canvas, game, 60);
    savePPM(canvas, "frame.ppm");
}
