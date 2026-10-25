// view3d.cpp — рейкастинг с текстурами
#include "view3d.hpp"

#include "raycast.hpp"
#include "textures.hpp"

#include <algorithm>
#include <cmath>

namespace doom {

namespace {
constexpr double kPlane = 0.8;    // полуширина окна камеры
constexpr double kNear = 0.05;    // ближе стену не считаем

// Три четверти яркости: половина плюс четверть, по всем каналам
// сразу. Маска не даёт битам перетечь в соседний канал.
std::uint32_t darker(std::uint32_t c) {
    return ((c >> 1) & 0x7F7F7F) + ((c >> 2) & 0x3F3F3F);
}
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
        const double cam = 2.0 * (col + 0.5) / w - 1.0;
        const double rayX = dirX + planeX * cam;
        const double rayY = dirY + planeY * cam;
        const RayHit hit = castRay(map, x, y, rayX, rayY);
        const double dist = std::max(hit.dist, kNear);
        const int wallH = static_cast<int>(h / dist);
        const int top = (h - wallH) / 2;
        if (wallH <= 0) {
            continue;                    // стена бесконечно далеко
        }

        // Где именно на грани попал луч: дробная часть координаты.
        double u = hit.side == 0 ? y + rayY * hit.dist
                                 : x + rayX * hit.dist;
        u -= std::floor(u);                          // 0 … 1
        const Image& tex = wallTexture(map.at(hit.cellX, hit.cellY));
        int texX = static_cast<int>(u * tex.width());
        // Чтобы текстура не была зеркальной на «обратных» гранях.
        if ((hit.side == 0 && rayX < 0.0) ||
            (hit.side == 1 && rayY > 0.0)) {
            texX = tex.width() - 1 - texX;
        }

        // Шаг по текстуре на один пиксель экрана.
        const double step = kTexSize / static_cast<double>(wallH);
        const int y0 = std::max(top, 0);
        const int y1 = std::min(top + wallH, h);
        double texPos = (y0 - top) * step;
        for (int py = y0; py < y1; ++py) {
            // & 63 — то же, что % 64, но быстрее и без минуса.
            const int texY =
                static_cast<int>(texPos) & (kTexSize - 1);
            texPos += step;
            const std::uint32_t c = tex.at(texX, texY);
            canvas.put(col, py, hit.side == 1 ? darker(c) : c);
        }
    }
}

} // namespace doom
