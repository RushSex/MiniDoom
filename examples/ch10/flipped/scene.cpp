// scene.cpp — адское небо, пол, колонны и статус-бар
#include "scene.hpp"

namespace doom {

namespace {
// Плавный переход от a к b: шаг t из n.
std::uint32_t mix(int a, int b, int t, int n) {
    return static_cast<std::uint32_t>(a + (b - a) * t / n);
}

std::uint32_t blend(std::uint32_t from, std::uint32_t to, int t,
                    int n) {
    const auto ch = [&](int shift) {
        const int a = static_cast<int>((from >> shift) & 0xFF);
        const int b = static_cast<int>((to >> shift) & 0xFF);
        return mix(a, b, t, n) << shift;
    };
    return ch(16) | ch(8) | ch(0);
}
} // namespace

void drawScene(Canvas& canvas) {
    const int w = canvas.width();
    const int h = canvas.height();
    const int horizon = (h - 64) / 2;
    for (int y = 0; y < horizon; ++y) {              // небо
        canvas.fillRect(0, y, w, 1, blend(rgb(30, 0, 0),
                        rgb(220, 70, 10), y, horizon));
    }
    for (int y = horizon; y < h - 64; ++y) {         // пол
        canvas.fillRect(0, y, w, 1, blend(rgb(90, 80, 70),
                        rgb(25, 22, 20), y - horizon, horizon));
    }
    for (int i = 0; i < 4; ++i) {                    // колонны
        const int x = 60 + i * 150;
        canvas.fillRect(x, 70, 60, 230, rgb(70, 70, 75));
        canvas.fillRect(x, 70, 12, 230, rgb(120, 120, 130));  // блик
    }
    canvas.fillRect(0, h - 64, w, 64, rgb(110, 100, 88));  // статус
    canvas.fillRect(20, h - 52, 100, 40, rgb(200, 30, 20));
    canvas.fillRect(140, h - 52, 160, 40, rgb(200, 30, 20));
}

} // namespace doom
