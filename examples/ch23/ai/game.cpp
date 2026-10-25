// game.cpp — ходьба, двери, предметы, монстры и стрельба
#include "game.hpp"

#include "art.hpp"
#include "raycast.hpp"
#include "sprites.hpp"
#include "topview.hpp"
#include "view3d.hpp"

#include <algorithm>
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
constexpr double kReach = 2.0;     // до какой двери дотянемся
constexpr double kDoorSpeed = 2.0; // открывается за 0,5 с
constexpr double kDoorWait = 3.0;  // столько стоит открытой
constexpr double kPickup = 0.5;    // с какого расстояния подбираем
constexpr double kMessage = 2.0;   // сколько секунд висит сообщение
constexpr double kMonsterRadius = 0.3;  // толщина монстра для пули
constexpr double kPain = 0.25;     // столько корчится от попадания
constexpr double kFxLife = 0.3;    // столько живёт облачко
} // namespace

Game::Game(Map map) : map_(std::move(map)) {
    const Cell start = map_.find('P').value_or(Cell{1, 1});
    x_ = start.x + 0.5;            // встаём в центр клетки
    y_ = start.y + 0.5;
    // Буквы h и a на карте — предметы. Кладём их в центры клеток.
    for (int cy = 0; cy < map_.height(); ++cy) {
        for (int cx = 0; cx < map_.width(); ++cx) {
            const char c = map_.at(cx, cy);
            if (c == 'h' || c == 'a') {
                const auto kind = c == 'h' ? ItemKind::Medkit
                                           : ItemKind::Clip;
                items_.push_back({kind, cx + 0.5, cy + 0.5});
            } else if (c == 'i' || c == 'z') {
                const auto kind = c == 'i' ? MonsterKind::Imp
                                           : MonsterKind::Zombie;
                // Разная фаза анимации, чтобы не шагали в ногу.
                const double phase = 0.37 * cx + 0.61 * cy;
                monsters_.push_back({kind, cx + 0.5, cy + 0.5, phase,
                                     specOf(kind).hp});
            }
        }
    }
}

void Game::update(const Input& in, double dt) {
    if (in.map) {
        showMap_ = !showMap_;
    }
    if (in.use) {
        useDoor();
    }
    updateDoors(dt);
    pickUpItems();
    updateMonsters(dt);
    for (Fx& f : fx_) {
        f.t += dt;
    }
    std::erase_if(fx_, [](const Fx& f) { return f.t > kFxLife; });
    messageTime_ = std::max(0.0, messageTime_ - dt);
    if (messageTime_ == 0.0) {
        message_.clear();
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
        tryMove(x_, y_, mx * step, my * step, kRadius);
        bob_ += dt * (in.run ? 13.0 : 10.0);
        bobAmp_ = std::min(1.0, bobAmp_ + dt * 4.0);
    } else {
        bobAmp_ = std::max(0.0, bobAmp_ - dt * 4.0);
    }
    updateWeapon(in, dt);
}

bool Game::blocked(double x, double y, double r) const {
    // Тело — квадратик 2r × 2r. Проверяем все клетки под ним.
    const int x0 = static_cast<int>(std::floor(x - r));
    const int x1 = static_cast<int>(std::floor(x + r));
    const int y0 = static_cast<int>(std::floor(y - r));
    const int y1 = static_cast<int>(std::floor(y + r));
    for (int cy = y0; cy <= y1; ++cy) {
        for (int cx = x0; cx <= x1; ++cx) {
            if (map_.isSolid(cx, cy)) {
                return true;
            }
        }
    }
    return false;
}

bool Game::touches(int cx, int cy) const {
    // Задевает ли квадратик игрока клетку (cx, cy)?
    const auto cell = [](double v) {
        return static_cast<int>(std::floor(v));
    };
    return cell(x_ - kRadius) <= cx && cx <= cell(x_ + kRadius) &&
           cell(y_ - kRadius) <= cy && cy <= cell(y_ + kRadius);
}

void Game::tryMove(double& x, double& y, double dx, double dy,
                   double r) const {
    // По осям по очереди: упёрлись по x — по y всё равно едем.
    if (!blocked(x + dx, y, r)) {
        x += dx;
    }
    if (!blocked(x, y + dy, r)) {
        y += dy;
    }
}

void Game::useDoor() {
    // Луч прямо по взгляду: если первой стоит дверь и она близко —
    // открываем.
    const RayHit hit =
        castRay(map_, x_, y_, std::cos(angle_), std::sin(angle_));
    Door* door = map_.door(hit.cellX, hit.cellY);
    if (door != nullptr && hit.dist < kReach) {
        door->move = 1;
    }
}

void Game::updateDoors(double dt) {
    for (int cy = 0; cy < map_.height(); ++cy) {
        for (int cx = 0; cx < map_.width(); ++cx) {
            Door* d = map_.door(cx, cy);
            if (d == nullptr) {
                continue;
            }
            if (d->move > 0) {                       // открывается
                d->open = std::min(1.0, d->open + kDoorSpeed * dt);
                if (d->open >= 1.0) {
                    d->move = 0;
                    d->wait = kDoorWait;
                }
            } else if (d->move < 0) {                // закрывается
                if (touches(cx, cy)) {
                    d->move = 1;                     // не прищемить!
                    continue;
                }
                d->open = std::max(0.0, d->open - kDoorSpeed * dt);
                if (d->open <= 0.0) {
                    d->move = 0;
                }
            } else if (d->open >= 1.0) {          // стоит открытой
                d->wait -= dt;
                if (d->wait <= 0.0 && !touches(cx, cy)) {
                    d->move = -1;
                }
            }
        }
    }
}

void Game::pickUpItems() {
    for (Item& item : items_) {
        const double dx = item.x - x_;
        const double dy = item.y - y_;
        if (item.taken || dx * dx + dy * dy > kPickup * kPickup) {
            continue;
        }
        if (item.kind == ItemKind::Medkit) {
            if (health_ >= 100) {
                continue;           // здоров — аптечка лежит дальше
            }
            health_ = std::min(100, health_ + 25);
            message_ = "PICKED UP A MEDIKIT";
        } else {
            ammo_ += 10;
            message_ = "PICKED UP A CLIP";
        }
        item.taken = true;
        messageTime_ = kMessage;
    }
}

void Game::updateWeapon(const Input& in, double dt) {
    if (in.weapon == 1) {
        weapon_ = Weapon::Pistol;
    } else if (in.weapon == 2) {
        weapon_ = Weapon::Shotgun;
    }
    cooldown_ = std::max(0.0, cooldown_ - dt);
    flash_ = std::max(0.0, flash_ - dt);
    const WeaponSpec& spec = specOf(weapon_);
    if (in.fire && cooldown_ == 0.0 && ammo_ >= spec.ammo) {
        fire();
    }
}

void Game::fire() {
    const WeaponSpec& spec = specOf(weapon_);
    ammo_ -= spec.ammo;
    cooldown_ = spec.cooldown;
    flash_ = 0.1;
    std::uniform_real_distribution<double> spread(-spec.spread,
                                                  spec.spread);
    std::uniform_int_distribution<int> damage(spec.minDamage,
                                              spec.maxDamage);
    for (int i = 0; i < spec.pellets; ++i) {
        hitscan(angle_ + spread(rng_), damage(rng_));
    }
}

void Game::hitscan(double angle, int damage) {
    const double dx = std::cos(angle);       // луч пули, длина 1
    const double dy = std::sin(angle);
    const RayHit wall = castRay(map_, x_, y_, dx, dy);
    Monster* target = nullptr;
    double best = wall.dist;                 // дальше стены не летит
    for (Monster& m : monsters_) {
        if (m.dead()) {
            continue;
        }
        const double rx = m.x - x_;
        const double ry = m.y - y_;
        const double along = rx * dx + ry * dy;     // вдоль луча
        const double off = std::abs(rx * dy - ry * dx); // поперёк
        if (along > 0.0 && along < best && off < kMonsterRadius) {
            best = along;                    // ближайший попавший
            target = &m;
        }
    }
    if (target == nullptr) {                 // пыль на стене
        const double d = wall.dist - 0.05;
        fx_.push_back({FxKind::Puff, x_ + dx * d, y_ + dy * d});
        return;
    }
    fx_.push_back({FxKind::Blood, x_ + dx * (best - 0.2),
                   y_ + dy * (best - 0.2)});
    target->hp -= damage;
    if (target->hp <= 0) {
        target->state = MonsterState::Dead;
        ++kills_;
    } else {                                 // больно — и зло
        target->state = MonsterState::Pain;
        target->timer = kPain;
    }
}

void Game::render(Canvas& canvas) const {
    if (showMap_) {
        drawTopView(canvas, map_, x_, y_, angle_);
        return;
    }
    const auto w = static_cast<std::size_t>(canvas.width());
    std::vector<double> zbuf(w);    // расстояние до стены в столбце
    drawView(canvas, map_, x_, y_, angle_, zbuf);
    std::vector<Sprite> sprites;
    for (const Item& item : items_) {
        if (!item.taken) {
            const Image* img = &itemImage(item.kind);
            sprites.push_back({item.x, item.y, img});
        }
    }
    for (const Monster& m : monsters_) {
        // Два кадра по очереди, 3 смены в секунду: переминается.
        const bool second = static_cast<int>(m.anim * 3.0) % 2 == 1;
        Pose pose = second ? Pose::Walk2 : Pose::Walk1;
        if (m.state == MonsterState::Dead) {
            pose = Pose::Dead;
        } else if (m.state == MonsterState::Pain) {
            pose = Pose::Pain;
        } else if (m.state == MonsterState::Attack) {
            pose = Pose::Attack;
        }
        sprites.push_back({m.x, m.y, &monsterImage(m.kind, pose)});
    }
    for (const Fx& f : fx_) {                // облачко растёт и тает
        const double grow = 1.0 + f.t * 2.0;
        const Image* img = &fxImage(f.kind);
        sprites.push_back({f.x, f.y, img, 0.0, grow, true});
    }
    drawSprites(canvas, std::move(sprites), {x_, y_, angle_}, zbuf);

    // Оружие качается при ходьбе и подпрыгивает при выстреле.
    const double swing = std::cos(bob_) * 10.0 * bobAmp_;
    const double hop = std::abs(std::sin(bob_)) * 8.0 * bobAmp_;
    const int bobX = static_cast<int>(swing);
    const int bobY = static_cast<int>(hop);
    const int recoil = static_cast<int>(cooldown_ * 14.0);
    drawWeapon(canvas, weapon_, flash_ > 0.0, bobX, bobY + recoil);
    const int cx = canvas.width() / 2;       // перекрестие
    const int cy = canvas.height() / 2;
    canvas.fillRect(cx - 6, cy, 4, 1, rgb(96, 255, 96));
    canvas.fillRect(cx + 3, cy, 4, 1, rgb(96, 255, 96));
    canvas.fillRect(cx, cy - 6, 1, 4, rgb(96, 255, 96));
    canvas.fillRect(cx, cy + 3, 1, 4, rgb(96, 255, 96));
}

void Game::teleport(double x, double y, double angle) {
    x_ = x;
    y_ = y;
    angle_ = angle;
}

} // namespace doom
