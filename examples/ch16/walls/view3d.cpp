// view3d.cpp — рейкастинг: по лучу на каждый столбец экрана
#include "view3d.hpp"

#include "raycast.hpp"

#include <algorithm>
#include <cmath>

namespace doom {

namespace {
constexpr double kPlane = 0.8;    // полуширина окна камеры
constexpr double kNear = 0.05;    // ближе стену не считаем
} // namespace

void drawView(Canvas& canvas, const Map& map, double x, double y,
              double angle) {
    const int w = canvas.width();
    const int h = canvas.height();
    // Верхняя половина — потолок, нижняя — пол.
    canvas.fillRect(0, 0, w, h / 2, rgb(50, 50, 60));
    canvas.fillRect(0, h / 2, w, h - h / 2, rgb(70, 62, 54));

    const double dirX = std::cos(angle);
    const double dirY = std::sin(angle);
    const double planeX = -dirY * kPlane;
    const double planeY = dirX * kPlane;
    for (int col = 0; col < w; ++col) {
        // Середина столбца на окне камеры: от −1 слева до +1 справа.
        const double cam = 2.0 * (col + 0.5) / w - 1.0;
        const RayHit hit = castRay(map, x, y, dirX + planeX * cam,
                                   dirY + planeY * cam);
        // dist — уже расстояние до стены вдоль взгляда (см. текст).
        const double dist = std::max(hit.dist, kNear);
        const int wallH = static_cast<int>(h / dist);
        const int top = (h - wallH) / 2;
        // Грани между рядами темнее: так видно, где угол.
        const auto color = hit.side == 0 ? rgb(176, 168, 156)
                                         : rgb(118, 112, 104);
        canvas.fillRect(col, top, 1, wallH, color);
    }
}

} // namespace doom
