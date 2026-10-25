// game.hpp — мир игры: карта, двери, предметы, монстры, оружие.
#pragma once

#include "art.hpp"
#include "canvas.hpp"
#include "input.hpp"
#include "items.hpp"
#include "map.hpp"
#include "monsters.hpp"
#include "weapons.hpp"

#include <random>
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
    [[nodiscard]] const std::vector<Monster>& monsters() const {
        return monsters_;
    }
    [[nodiscard]] Weapon weapon() const { return weapon_; }
    [[nodiscard]] int kills() const { return kills_; }
    // Только для тестов и отладки: поставить игрока куда угодно.
    void teleport(double x, double y, double angle);
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
    void updateWeapon(const Input& in, double dt);
    void fire();
    void hitscan(double angle, int damage);

    Map map_;
    double x_ = 0.0;         // в клетках: 1.5 — центр клетки 1
    double y_ = 0.0;
    double angle_ = 0.0;     // 0 — восток, растёт по часовой
    bool showMap_ = false;   // показывать карту вместо вида из глаз
    int health_ = 80;        // проценты, как в Doom
    int ammo_ = 20;
    std::vector<Item> items_;
    std::vector<Monster> monsters_;
    std::vector<Fx> fx_;
    Weapon weapon_ = Weapon::Pistol;
    double cooldown_ = 0.0;  // до следующего выстрела
    double flash_ = 0.0;     // сколько ещё горит вспышка
    double bob_ = 0.0;       // фаза покачивания оружия
    double bobAmp_ = 0.0;    // размах покачивания: 0 стоим, 1 идём
    int kills_ = 0;
    std::mt19937 rng_{2026}; // свой генератор: тесты повторяемы
    std::string message_;
    double messageTime_ = 0.0;  // сколько ещё показывать
};

} // namespace doom
