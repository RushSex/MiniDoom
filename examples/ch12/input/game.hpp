// game.hpp — мир игры: игрок и прицел. Про Windows не знает.
#pragma once

#include "canvas.hpp"
#include "input.hpp"

namespace doom {

class Game {
public:
    Game(int width, int height);

    void update(const Input& in, double dt);
    void render(Canvas& canvas) const;

    [[nodiscard]] double playerX() const { return x_; }
    [[nodiscard]] double playerY() const { return y_; }
    [[nodiscard]] double aimX() const { return aimX_; }
    [[nodiscard]] double aimY() const { return aimY_; }
    [[nodiscard]] int shots() const { return shots_; }

private:
    int width_;
    int height_;
    double x_;              // центр игрока
    double y_;
    double aimX_;           // прицел
    double aimY_;
    double flash_ = 0.0;    // сколько ещё горит вспышка, с
    int shots_ = 0;
};

} // namespace doom
