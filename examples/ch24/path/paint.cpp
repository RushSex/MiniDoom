// paint.cpp — эллипс, многоугольник, обводка, подкраска
#include "paint.hpp"

#include "art.hpp"
#include "canvas.hpp"

#include <algorithm>

namespace doom {

std::uint32_t lit(std::uint32_t base, double k) {
    const auto ch = [k](std::uint32_t v) {
        const double s = std::clamp(v * k, 0.0, 255.0);
        return static_cast<std::uint32_t>(s);
    };
    const std::uint32_t r = ch((base >> 16) & 0xFF);
    const std::uint32_t g = ch((base >> 8) & 0xFF);
    const std::uint32_t b = ch(base & 0xFF);
    return opaque(rgb(r, g, b));
}

void fillEllipse(Image& img, Point c, double rx, double ry,
                 std::uint32_t base) {
    forEachPixel(img, [&](int x, int y) {
        // Координаты внутри эллипса: от −1 до 1 по каждой оси.
        const double nx = (x + 0.5 - c.x) / rx;
        const double ny = (y + 0.5 - c.y) / ry;
        const double d = nx * nx + ny * ny;
        if (d > 1.0) {
            return;                       // снаружи
        }
        // Слева сверху светлее, к краям темнее — шар, а не блин.
        const double light = 1.1 - 0.3 * (nx + ny) - 0.3 * d;
        img.set(x, y, lit(base, light));
    });
}

void fillPolygon(Image& img, std::span<const Point> pts,
                 std::uint32_t base) {
    forEachPixel(img, [&](int x, int y) {
        const Point p{x + 0.5, y + 0.5};
        bool left = false;                // точка слева от ребра
        bool right = false;               // точка справа от ребра
        for (std::size_t i = 0; i < pts.size(); ++i) {
            const Point a = pts[i];
            const Point b = pts[(i + 1) % pts.size()];
            // Векторное произведение: знак говорит, с какой стороны.
            const double cross = (b.x - a.x) * (p.y - a.y) -
                                 (b.y - a.y) * (p.x - a.x);
            left = left || cross < 0.0;
            right = right || cross > 0.0;
        }
        if (!(left && right)) {           // со всех сторон одинаково
            img.set(x, y, lit(base, 1.0));
        }
    });
}

void outline(Image& img, std::uint32_t base) {
    const Image src = img;                // смотрим на копию «до»
    const auto solid = [&src](int x, int y) {
        return x >= 0 && y >= 0 && x < src.width() &&
               y < src.height() && (src.at(x, y) >> 24) != 0;
    };
    forEachPixel(img, [&](int x, int y) {
        if (solid(x, y)) {
            return;
        }
        if (solid(x - 1, y) || solid(x + 1, y) || solid(x, y - 1) ||
            solid(x, y + 1)) {
            img.set(x, y, opaque(base));
        }
    });
}

void tint(Image& img, std::uint32_t base, double amount) {
    forEachPixel(img, [&](int x, int y) {
        const std::uint32_t c = img.at(x, y);
        if ((c >> 24) == 0) {
            return;
        }
        const auto mix = [&](int shift) {
            const double a = (c >> shift) & 0xFF;
            const double b = (base >> shift) & 0xFF;
            return static_cast<std::uint32_t>(a + (b - a) * amount);
        };
        img.set(x, y, opaque(rgb(mix(16), mix(8), mix(0))));
    });
}

} // namespace doom
