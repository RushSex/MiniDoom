// test.cpp — рисуем сцену без окна и проверяем пиксели
#include "canvas.hpp"
#include "scene.hpp"

#include <format>
#include <fstream>
#include <iostream>

namespace {
// Сохраняет холст в простейший формат картинок PPM.
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
    doom::Canvas canvas(640, 400);
    doom::drawScene(canvas);
    canvas.put(-5, 10, 0xFFFFFF);          // за краем — безопасно
    canvas.put(700, 900, 0xFFFFFF);
    const auto show = [&](const char* name, int x, int y) {
        std::cout << std::format("{:<10} ({:>3},{:>3}) = {:06X}\n",
                                 name, x, y, canvas.at(x, y));
    };
    show("sky top", 0, 0);
    show("sky low", 0, 167);
    show("pillar", 90, 100);
    show("status", 30, 380);
    savePPM(canvas, "frame.ppm");
    std::cout << "frame.ppm saved\n";
}
