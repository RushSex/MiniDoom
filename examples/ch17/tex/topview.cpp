// topview.cpp — карта сверху и «фонарик» из лучей
#include "topview.hpp"

#include "raycast.hpp"

#include <cmath>
#include <cstddef>
#include <vector>

namespace doom {

namespace {
constexpr int kCell = 22;       // пикселей на клетку
constexpr int kRays = 80;       // сколько лучей бросаем
constexpr double kPlane = 0.8;  // полуширина «окна» камеры
} // namespace

void drawTopView(Canvas& canvas, const Map& map, double x, double y,
                 double angle) {
    const double dirX = std::cos(angle);
    const double dirY = std::sin(angle);
    // Лучи веером: от левого края «окна» камеры до правого.
    std::vector<RayHit> hits;
    std::vector<double> rx, ry;
    for (int i = 0; i < kRays; ++i) {
        const double cam = 2.0 * i / (kRays - 1) - 1.0;  // −1 … +1
        rx.push_back(dirX - dirY * kPlane * cam);
        ry.push_back(dirY + dirX * kPlane * cam);
        hits.push_back(castRay(map, x, y, rx.back(), ry.back()));
    }

    canvas.clear(rgb(16, 16, 20));
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
    for (const RayHit& h : hits) {        // стены, куда попал луч
        canvas.fillRect(ox + h.cellX * kCell, oy + h.cellY * kCell,
                        kCell - 1, kCell - 1, rgb(220, 190, 90));
    }
    const auto px = [&](double v) {
        return ox + static_cast<int>(std::lround(v * kCell));
    };
    const auto py = [&](double v) {
        return oy + static_cast<int>(std::lround(v * kCell));
    };
    const auto beam = rgb(255, 230, 120);
    for (std::size_t i = 0; i < hits.size(); ++i) {
        // Точка попадания: начало + направление × расстояние.
        const double hx = x + rx[i] * hits[i].dist;
        const double hy = y + ry[i] * hits[i].dist;
        canvas.line(px(x), py(y), px(hx), py(hy), beam);
    }
    canvas.fillRect(px(x) - 6, py(y) - 6, 13, 13, rgb(230, 70, 20));
}

} // namespace doom
