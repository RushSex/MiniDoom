// textures.cpp — камень, кирпич и металл из шума и арифметики
#include "textures.hpp"

#include "canvas.hpp"
#include "noise.hpp"

#include <algorithm>
#include <array>
#include <cmath>

namespace doom {

namespace {
// Цвет из дробных каналов 0…255 с защитой от выхода за край.
std::uint32_t color(double r, double g, double b) {
    const auto c = [](double v) {
        return static_cast<std::uint32_t>(std::clamp(v, 0.0, 255.0));
    };
    return rgb(c(r), c(g), c(b));
}
} // namespace

Image makeStone() {
    Image img(kTexSize, kTexSize);
    for (int y = 0; y < kTexSize; ++y) {
        for (int x = 0; x < kTexSize; ++x) {
            // Пятна + мелкая крошка; узор повторяется каждые 64.
            const double n = fbm(x / 16.0, y / 16.0, 4, 4, 11);
            const double v = 60.0 + 150.0 * n;
            img.set(x, y, color(v, v * 0.97, v * 0.9));
        }
    }
    return img;
}

Image makeBricks() {
    Image img(kTexSize, kTexSize);
    for (int y = 0; y < kTexSize; ++y) {
        const int row = y / 16;             // ряд кирпичей
        const int shift = (row % 2) * 16;   // нечётные ряды сдвинуты
        for (int x = 0; x < kTexSize; ++x) {
            const int bx = (x + shift) % kTexSize;
            const bool mortar = y % 16 < 2 || bx % 32 < 2;
            const double n = fbm(x / 8.0, y / 8.0, 3, 8, 22);
            if (mortar) {                       // раствор
                const double v = 90.0 + 50.0 * n;
                img.set(x, y, color(v, v, v * 0.95));
                continue;
            }
            // Каждый кирпич чуть своего оттенка: хеш от его номера.
            const double tone = 0.8 + 0.35 * hash01(bx / 32, row, 3);
            const double v = (0.7 + 0.5 * n) * tone;
            img.set(x, y, color(150.0 * v, 62.0 * v, 44.0 * v));
        }
    }
    return img;
}

Image makeTech() {
    Image img(kTexSize, kTexSize);
    for (int y = 0; y < kTexSize; ++y) {
        for (int x = 0; x < kTexSize; ++x) {
            const int px = x % 32;                 // панель 32 × 32
            const int py = y % 32;
            const double n = fbm(x / 16.0, y / 16.0, 3, 4, 44);
            double v = 0.75 + 0.3 * n;             // потёртый металл
            if (px == 0 || py == 0) {
                v = 0.35;                          // шов: тёмный
            } else if (px == 1 || py == 1) {
                v = 1.25;                          // кромка: блик
            }
            const bool rivet = (px == 4 || px == 28) &&
                               (py == 4 || py == 28);
            if (rivet) {
                v = 1.4;                           // заклёпки
            }
            if (y % 32 >= 14 && y % 32 <= 17 && px > 6 && px < 26) {
                img.set(x, y, color(230.0, 170.0, 40.0));  // полоса
                continue;
            }
            img.set(x, y, color(92.0 * v, 104.0 * v, 118.0 * v));
        }
    }
    return img;
}

const Image& wallTexture(char cell) {
    // Строится один раз, при первом вызове: static внутри функции.
    static const std::array<Image, 3> textures{makeStone(),
                                               makeBricks(),
                                               makeTech()};
    switch (cell) {
    case '2':
        return textures[1];
    case '3':
        return textures[2];
    default:
        return textures[0];
    }
}

} // namespace doom
