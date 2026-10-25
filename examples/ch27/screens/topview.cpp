// topview.cpp — автокарта: открывается по мере того, как смотришь
#include "topview.hpp"

#include <cmath>
#include <utility>

namespace doom {

namespace {
constexpr int kCell = 20;       // пикселей на клетку
} // namespace

void drawTopView(Canvas& canvas, const Game& game) {
    const Map& map = game.map();
    canvas.clear(rgb(10, 10, 14));
    const int ox = (canvas.width() - map.width() * kCell) / 2;
    const int oy = (canvas.height() - Game::kStatusBar -
                    map.height() * kCell) / 2;
    for (int cy = 0; cy < map.height(); ++cy) {
        for (int cx = 0; cx < map.width(); ++cx) {
            if (!game.seen(cx, cy)) {
                continue;               // не видели — белое пятно
            }
            auto color = map.isWall(cx, cy) ? rgb(150, 150, 160)
                                            : rgb(36, 36, 44);
            if (map.door(cx, cy) != nullptr) {
                color = rgb(200, 120, 40);
            } else if (map.at(cx, cy) == 'X') {
                color = rgb(60, 200, 80);   // выход
            }
            canvas.fillRect(ox + cx * kCell, oy + cy * kCell,
                            kCell - 1, kCell - 1, color);
        }
    }
    // Игрок — стрелка: нос по взгляду, два «крыла» назад.
    const double a = game.angle();
    const auto at = [&](double ang, double r) {
        const double x = game.playerX() + std::cos(a + ang) * r;
        const double y = game.playerY() + std::sin(a + ang) * r;
        const auto px = static_cast<int>(std::lround(x * kCell));
        const auto py = static_cast<int>(std::lround(y * kCell));
        return std::pair{ox + px, oy + py};
    };
    const auto [nx, ny] = at(0.0, 0.5);
    const auto [lx, ly] = at(2.5, 0.4);
    const auto [rx, ry] = at(-2.5, 0.4);
    const auto red = rgb(255, 80, 40);
    canvas.line(nx, ny, lx, ly, red);
    canvas.line(nx, ny, rx, ry, red);
    canvas.line(lx, ly, rx, ry, red);
}

} // namespace doom
