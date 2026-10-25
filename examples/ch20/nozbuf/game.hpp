// game.hpp — мир игры: карта, двери, предметы и игрок.
#pragma once

#include "canvas.hpp"
#include "input.hpp"
#include "items.hpp"
#include "map.hpp"

#include <string>
#include <vector>

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
    [[nodiscard]] int health() const { return health_; }
    [[nodiscard]] int ammo() const { return ammo_; }
    [[nodiscard]] const std::vector<Item>& items() const {
        return items_;
    }
    // Сообщение вверху экрана; пустое, если показывать нечего.
    [[nodiscard]] const std::string& message() const {
        return message_;
    }

private:
    [[nodiscard]] bool blocked(double x, double y) const;
    [[nodiscard]] bool touches(int cx, int cy) const;
    void tryMove(double dx, double dy);
    void useDoor();
    void updateDoors(double dt);
    void pickUpItems();

    Map map_;
    double x_ = 0.0;         // в клетках: 1.5 — центр клетки 1
    double y_ = 0.0;
    double angle_ = 0.0;     // 0 — восток, растёт по часовой
    bool showMap_ = false;   // показывать карту вместо вида из глаз
    int health_ = 80;        // проценты, как в Doom
    int ammo_ = 20;
    std::vector<Item> items_;
    std::string message_;
    double messageTime_ = 0.0;  // сколько ещё показывать
};

} // namespace doom
