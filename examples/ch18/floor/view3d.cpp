// view3d.cpp — пол, потолок и стены; всё тонет в темноте
#include "view3d.hpp"

#include "raycast.hpp"
#include "shade.hpp"
#include "textures.hpp"

#include <algorithm>
#include <cmath>

namespace doom {

namespace {
constexpr double kPlane = 0.8;    // полуширина окна камеры
constexpr double kNear = 0.05;    // ближе стену не считаем

// Пол и потолок: строка за строкой, от горизонта к краю экрана.
void drawFloorCeiling(Canvas& canvas, double x, double y,
                      double dirX, double dirY) {
    const int w = canvas.width();
    const int h = canvas.height();
    const double half = h / 2.0;
    // Крайние лучи: через левый и правый края окна камеры.
    const double lx = dirX + dirY * kPlane;
    const double ly = dirY - dirX * kPlane;
    const double rx = dirX - dirY * kPlane;
    const double ry = dirY + dirX * kPlane;
    const Image& floorTex = floorTexture();
    const Image& ceilTex = ceilingTexture();
    for (int row = h / 2; row < h; ++row) {
        // На каком расстоянии пол виден в этой строке экрана.
        const double rowDist = half / (row - half + 0.5);
        // Точка пола под левым краем и шаг вправо на один пиксель.
        const double stepX = rowDist * (rx - lx) / w;
        const double stepY = rowDist * (ry - ly) / w;
        double fx = x + rowDist * lx + stepX * 0.5;
        double fy = y + rowDist * ly + stepY * 0.5;
        const std::uint32_t light = lightAt(rowDist);
        const int ceilRow = h - 1 - row;           // зеркально вверх
        for (int col = 0; col < w; ++col) {
            const int tx = static_cast<int>(fx * kTexSize) & 63;
            const int ty = static_cast<int>(fy * kTexSize) & 63;
            fx += stepX;
            fy += stepY;
            canvas.put(col, row, shade(floorTex.at(tx, ty), light));
            canvas.put(col, ceilRow,
                       shade(ceilTex.at(tx, ty), light));
        }
    }
}
} // namespace

void drawView(Canvas& canvas, const Map& map, double x, double y,
              double angle) {
    const int w = canvas.width();
    const int h = canvas.height();
    const double dirX = std::cos(angle);
    const double dirY = std::sin(angle);
    drawFloorCeiling(canvas, x, y, dirX, dirY);

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
            continue;
        }
        double u = hit.side == 0 ? y + rayY * hit.dist
                                 : x + rayX * hit.dist;
        u -= std::floor(u);
        const Image& tex = wallTexture(map.at(hit.cellX, hit.cellY));
        int texX = static_cast<int>(u * kTexSize);
        if ((hit.side == 0 && rayX < 0.0) ||
            (hit.side == 1 && rayY > 0.0)) {
            texX = kTexSize - 1 - texX;
        }
        // Свет по расстоянию; грани side = 1 ещё на четверть темнее.
        std::uint32_t light = lightAt(dist);
        if (hit.side == 1) {
            light = light * 3 / 4;
        }
        const double step = kTexSize / static_cast<double>(wallH);
        const int y0 = std::max(top, 0);
        const int y1 = std::min(top + wallH, h);
        double texPos = (y0 - top) * step;
        for (int py = y0; py < y1; ++py) {
            const int texY = static_cast<int>(texPos) & 63;
            texPos += step;
            canvas.put(col, py, shade(tex.at(texX, texY), light));
        }
    }
}

} // namespace doom
