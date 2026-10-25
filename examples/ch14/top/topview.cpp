// topview.cpp — рисуем карту и игрока, вид сверху
#include "topview.hpp"

#include <cmath>

namespace doom {

namespace {
constexpr int kCell = 22;                 // пикселей на клетку
} // namespace

void drawTopView(Canvas& canvas, const Map& map, double x, double y,
                 double angle) {
    canvas.clear(rgb(16, 16, 20));
    // Карта по центру холста.
    const int ox = (canvas.width() - map.width() * kCell) / 2;
    const int oy = (canvas.height() - map.height() * kCell) / 2;
    for (int cy = 0; cy < map.height(); ++cy) {
        for (int cx = 0; cx < map.width(); ++cx) {
            const auto color = map.isWall(cx, cy)
                                   ? rgb(150, 150, 160)
                                   : rgb(40, 40, 48);
            canvas.fillRect(ox + cx * kCell, oy + cy * kCell,
                            kCell - 1, kCell - 1, color);
        }
    }
    // Клетки → пиксели: умножаем на размер клетки.
    const auto px = [&](double v) {
        return ox + static_cast<int>(std::lround(v * kCell));
    };
    const auto py = [&](double v) {
        return oy + static_cast<int>(std::lround(v * kCell));
    };
    const double dirX = std::cos(angle);
    const double dirY = std::sin(angle);
    // Линия толщиной в 2 пикселя: две рядом. Жёлтая — взгляд,
    // синяя — направление «вправо».
    const auto thick = [&](double tx, double ty, std::uint32_t c) {
        canvas.line(px(x), py(y), px(tx), py(ty), c);
        canvas.line(px(x) + 1, py(y) + 1, px(tx) + 1, py(ty) + 1, c);
    };
    thick(x + dirX, y + dirY, rgb(255, 220, 60));      // вперёд
    thick(x - dirY * 0.6, y + dirX * 0.6, rgb(90, 160, 255));
    canvas.fillRect(px(x) - 6, py(y) - 6, 13, 13, rgb(230, 70, 20));
}

} // namespace doom
