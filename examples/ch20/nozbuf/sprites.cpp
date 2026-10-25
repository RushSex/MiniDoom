// sprites.cpp — проекция, сортировка и Z-буфер
#include "sprites.hpp"

#include "shade.hpp"
#include "view3d.hpp"

#include <algorithm>
#include <cmath>
#include <functional>

namespace doom {

void drawSprites(Canvas& canvas, std::vector<Sprite> sprites,
                 const Camera& cam, std::span<const double>) {
    const double w = canvas.width();
    const double h = canvas.height();
    const double dirX = std::cos(cam.angle);
    const double dirY = std::sin(cam.angle);
    for (Sprite& s : sprites) {         // насколько далеко впереди
        s.depth = (s.x - cam.x) * dirX + (s.y - cam.y) * dirY;
    }
    // Художник: сначала дальние, ближние рисуются поверх.
    std::ranges::sort(sprites, std::greater{}, &Sprite::depth);

    for (const Sprite& s : sprites) {
        const double d = s.depth;
        if (d < 0.1) {
            continue;                   // позади или в упор
        }
        // Насколько правее взгляда (в клетках) — и куда на экран.
        const double rx = s.x - cam.x;
        const double ry = s.y - cam.y;
        const double side = ry * dirX - rx * dirY;
        const double mid = w / 2 * (1 + side / (kPlane * d));
        const double size = h / d;      // клетка на таком удалении
        const double left = mid - size / 2;
        const double top = h / 2 + h / 2 / d - size; // низ — на полу
        const double step = s.image->width() / size;
        const std::uint32_t light = lightAt(d);

        // Прямоугольник спрайта на экране, обрезанный по краям.
        const auto px = [](double v) { return static_cast<int>(v); };
        const int x0 = std::max(0, px(std::ceil(left)));
        const int x1 = std::min(px(w), px(left + size));
        const int y0 = std::max(0, px(std::ceil(top)));
        const int y1 = std::min(px(h), px(top + size));
        for (int col = x0; col < x1; ++col) {
            // ОШИБКА: забыли проверить zbuf — стены не закрывают.
            const int tx = px((col - left) * step) & 63;
            for (int row = y0; row < y1; ++row) {
                const int ty = px((row - top) * step) & 63;
                const std::uint32_t c = s.image->at(tx, ty);
                if ((c >> 24) != 0) {   // альфа 0 — прозрачно
                    canvas.put(col, row, shade(c, light));
                }
            }
        }
    }
}

} // namespace doom
