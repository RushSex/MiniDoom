// game.hpp — мир игры: карта, двери и игрок. Про Windows не знает.
#pragma once

#include "canvas.hpp"
#include "input.hpp"
#include "map.hpp"

namespace doom {

class Game {
public:
    explicit Game(Map map);

    void update(const Input& in, double dt);
    void render(Canvas& canvas) const;

    [[nodiscard]] double playerX() const { return x_; }
    [[nodiscard]] double playerY() const { return y_; }
    [[nodiscard]] double angle() const { return angle_; }  // радианы
    [[nodiscard]] const Map& map() const { return map_; }

private:
    [[nodiscard]] bool blocked(double x, double y) const;
    [[nodiscard]] bool touches(int cx, int cy) const;
    void tryMove(double dx, double dy);
    void useDoor();
    void updateDoors(double dt);

    Map map_;
    double x_ = 0.0;         // в клетках: 1.5 — центр клетки 1
    double y_ = 0.0;
    double angle_ = 0.0;     // 0 — восток, растёт по часовой
    bool showMap_ = false;   // показывать карту вместо вида из глаз
};

} // namespace doom
