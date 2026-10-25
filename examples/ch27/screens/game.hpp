// game.hpp — мир игры: карта, двери, предметы, монстры, оружие.
#pragma once

#include "art.hpp"
#include "canvas.hpp"
#include "input.hpp"
#include "items.hpp"
#include "map.hpp"
#include "monsters.hpp"
#include "sound.hpp"
#include "weapons.hpp"

#include <cstdint>
#include <random>
#include <string>
#include <utility>
#include <vector>

namespace doom {

class Game {
public:
    // Высота статус-бара внизу экрана: оружие рисуем над ним.
    static constexpr int kStatusBar = 44;

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
    [[nodiscard]] bool dead() const { return dead_; }
    [[nodiscard]] double deadTime() const { return deadTime_; }
    [[nodiscard]] double hurt() const { return hurt_; }
    // Для экрана итогов: дошёл до выхода, время, счёт.
    [[nodiscard]] bool levelDone() const { return levelDone_; }
    [[nodiscard]] double time() const { return time_; }
    [[nodiscard]] int itemsTaken() const;
    [[nodiscard]] int itemsTotal() const;
    [[nodiscard]] int monstersTotal() const {
        return static_cast<int>(monsters_.size());
    }
    [[nodiscard]] bool mapShown() const { return showMap_; }
    // Видел ли игрок клетку — для автокарты.
    [[nodiscard]] bool seen(int cx, int cy) const;
    [[nodiscard]] const std::vector<Projectile>& shots() const {
        return shots_;
    }
    // Поле расстояний до игрока, по числу на клетку карты.
    [[nodiscard]] const std::vector<int>& flow() const {
        return flow_;
    }
    // Только для тестов и отладки: поставить игрока куда угодно.
    void teleport(double x, double y, double angle);
    // Звуки, прозвучавшие с прошлого вызова; очередь очищается.
    [[nodiscard]] std::vector<SoundEvent> takeSounds() {
        return std::exchange(sounds_, {});
    }
    // Сообщение вверху экрана; пустое, если показывать нечего.
    [[nodiscard]] const std::string& message() const {
        return message_;
    }

private:
    // Тело радиуса r в точке (x, y) задевает стену или дверь?
    [[nodiscard]] bool blocked(double x, double y, double r) const;
    [[nodiscard]] bool touches(int cx, int cy) const;
    // В клетке стоит игрок или живой монстр: двери не закрываться.
    [[nodiscard]] bool occupied(int cx, int cy) const;
    // Сдвинуть тело (x, y) на (dx, dy) со скольжением вдоль стен.
    void tryMove(double& x, double& y, double dx, double dy,
                 double r) const;
    // Звук в точке (x, y): чем дальше от игрока, тем тише.
    void sound(SoundId id, double x, double y);
    void openDoor(Door& door, int cx, int cy);
    void wake(Monster& m);
    void useDoor();
    void updateDoors(double dt);
    void pickUpItems();
    void updateWeapon(const Input& in, double dt);
    void fire();
    void hitscan(double angle, int damage);
    void updatePlayer(const Input& in, double dt);
    void updateProjectiles(double dt);
    void lookAround();
    void restart();
    // Мозги монстров — в ai.cpp.
    void updateMonsters(double dt);
    void updateMonster(Monster& m, double dt);
    [[nodiscard]] bool canSee(const Monster& m) const;
    void monsterAttack(const Monster& m, double dist);
    void hurtPlayer(int damage);
    void followFlow(Monster& m, double step);
    void wakeByNoise();

    Map initial_;            // карта как в начале — для рестарта
    Map map_;                // карта сейчас: двери двигаются
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
    std::vector<Projectile> shots_;
    double hurt_ = 0.0;      // красная вспышка: 0 — нет, 1 — сильная
    bool dead_ = false;
    double deadTime_ = 0.0;  // сколько секунд игрок мёртв
    std::vector<int> flow_;  // BFS-расстояния до игрока, в шагах
    double flowTimer_ = 0.0; // до следующего пересчёта поля
    bool noise_ = false;     // в этом кадре был выстрел
    std::mt19937 rng_{2026}; // свой генератор: тесты повторяемы
    std::vector<SoundEvent> sounds_;  // для микшера
    bool levelDone_ = false;
    double time_ = 0.0;      // секунд на уровне
    std::vector<std::uint8_t> seen_;  // 1 — клетку уже видели
    std::string message_;
    double messageTime_ = 0.0;  // сколько ещё показывать
};

} // namespace doom
