// game.cpp — правила движения и рисование мира
#include "game.hpp"

#include <algorithm>

namespace doom {

namespace {
constexpr int kSize = 24;              // сторона квадрата-игрока
constexpr double kWalk = 180.0;        // пикселей в секунду
constexpr double kRun = 320.0;
constexpr double kMouse = 0.5;         // чувствительность мыши
constexpr double kDiagonal = 0.7071;   // 1/√2: наискосок не быстрее
} // namespace

Game::Game(int width, int height)
    : width_(width), height_(height), x_(width / 2.0),
      y_(height / 2.0), aimX_(width * 0.75), aimY_(height / 2.0) {}

void Game::update(const Input& in, double dt) {
    double dx = (in.right ? 1.0 : 0.0) - (in.left ? 1.0 : 0.0);
    double dy = (in.down ? 1.0 : 0.0) - (in.up ? 1.0 : 0.0);
    if (dx != 0.0 && dy != 0.0) {
        dx *= kDiagonal;
        dy *= kDiagonal;
    }
    const double speed = in.run ? kRun : kWalk;
    const double half = kSize / 2.0;
    x_ = std::clamp(x_ + dx * speed * dt, half, width_ - half);
    y_ = std::clamp(y_ + dy * speed * dt, half, height_ - half);

    aimX_ = std::clamp(aimX_ + in.mouseDX * kMouse, 0.0,
                       width_ - 1.0);
    aimY_ = std::clamp(aimY_ + in.mouseDY * kMouse, 0.0,
                       height_ - 1.0);

    flash_ = std::max(0.0, flash_ - dt);
    if (in.fire) {
        ++shots_;
        flash_ = 0.1;
    }
}

void Game::render(Canvas& canvas) const {
    canvas.clear(rgb(20, 20, 28));
    for (int x = 0; x < width_; x += 40) {          // сетка пола
        canvas.fillRect(x, 0, 1, height_, rgb(40, 40, 52));
    }
    for (int y = 0; y < height_; y += 40) {
        canvas.fillRect(0, y, width_, 1, rgb(40, 40, 52));
    }
    const auto color = flash_ > 0.0 ? rgb(255, 230, 120)
                                    : rgb(230, 70, 20);
    canvas.fillRect(static_cast<int>(x_) - kSize / 2,
                    static_cast<int>(y_) - kSize / 2, kSize, kSize,
                    color);
    const int ax = static_cast<int>(aimX_);         // прицел-крестик
    const int ay = static_cast<int>(aimY_);
    canvas.fillRect(ax - 8, ay, 6, 1, rgb(90, 255, 90));
    canvas.fillRect(ax + 3, ay, 6, 1, rgb(90, 255, 90));
    canvas.fillRect(ax, ay - 8, 1, 6, rgb(90, 255, 90));
    canvas.fillRect(ax, ay + 3, 1, 6, rgb(90, 255, 90));
}

} // namespace doom
