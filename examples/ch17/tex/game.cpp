// game.cpp — ходьба как в главе 14, плюс выбор вида
#include "game.hpp"

#include "topview.hpp"
#include "view3d.hpp"

#include <cmath>
#include <numbers>
#include <utility>

namespace doom {

namespace {
constexpr double kWalk = 3.0;      // клеток в секунду
constexpr double kRun = 5.5;
constexpr double kTurn = 2.5;      // радиан в секунду — стрелками
constexpr double kMouse = 0.0025;  // радиан на единицу мыши
constexpr double kRadius = 0.25;   // половина ширины игрока
} // namespace

Game::Game(Map map) : map_(std::move(map)) {
    const Cell start = map_.find('P').value_or(Cell{1, 1});
    x_ = start.x + 0.5;            // встаём в центр клетки
    y_ = start.y + 0.5;
}

void Game::update(const Input& in, double dt) {
    if (in.map) {
        showMap_ = !showMap_;
    }
    const double turn = (in.turnRight ? 1.0 : 0.0) -
                        (in.turnLeft ? 1.0 : 0.0);
    angle_ += turn * kTurn * dt + in.mouseDX * kMouse;
    angle_ = std::remainder(angle_, 2.0 * std::numbers::pi);

    const double fwd = (in.forward ? 1.0 : 0.0) -
                       (in.back ? 1.0 : 0.0);
    const double side = (in.strafeRight ? 1.0 : 0.0) -
                        (in.strafeLeft ? 1.0 : 0.0);
    const double dirX = std::cos(angle_);       // куда смотрим
    const double dirY = std::sin(angle_);
    // Вперёд — вдоль (dirX, dirY), вправо — вдоль (-dirY, dirX).
    const double mx = dirX * fwd - dirY * side;
    const double my = dirY * fwd + dirX * side;
    const double len = std::hypot(mx, my);
    if (len > 0.0) {
        const double step = (in.run ? kRun : kWalk) * dt / len;
        tryMove(mx * step, my * step);
    }
}

bool Game::blocked(double x, double y) const {
    // Игрок — квадратик 2r × 2r. Проверяем все клетки под ним.
    const int x0 = static_cast<int>(std::floor(x - kRadius));
    const int x1 = static_cast<int>(std::floor(x + kRadius));
    const int y0 = static_cast<int>(std::floor(y - kRadius));
    const int y1 = static_cast<int>(std::floor(y + kRadius));
    for (int cy = y0; cy <= y1; ++cy) {
        for (int cx = x0; cx <= x1; ++cx) {
            if (map_.isWall(cx, cy)) {
                return true;
            }
        }
    }
    return false;
}

void Game::tryMove(double dx, double dy) {
    // По осям по очереди: упёрлись по x — по y всё равно едем.
    if (!blocked(x_ + dx, y_)) {
        x_ += dx;
    }
    if (!blocked(x_, y_ + dy)) {
        y_ += dy;
    }
}

void Game::render(Canvas& canvas) const {
    if (showMap_) {
        drawTopView(canvas, map_, x_, y_, angle_);
    } else {
        drawView(canvas, map_, x_, y_, angle_);
    }
}

} // namespace doom
