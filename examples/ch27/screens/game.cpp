// game.cpp — игрок, двери, предметы, стрельба, снаряды и смерть
#include "game.hpp"

#include "art.hpp"
#include "pathfind.hpp"
#include "raycast.hpp"
#include "shade.hpp"
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
constexpr double kFlowPeriod = 0.25; // поле: 4 пересчёта в секунду
constexpr double kSubstep = 0.1;   // снаряд за подшаг — не дальше
constexpr double kHitPlayer = 0.35;  // шар задел игрока
constexpr double kEarshot = 16.0;  // дальше — не слышно
} // namespace

Game::Game(Map map) : initial_(map), map_(std::move(map)) {
    const Cell start = map_.find('P').value_or(Cell{1, 1});
    x_ = start.x + 0.5;            // встаём в центр клетки
    y_ = start.y + 0.5;
    // Буквы h и a на карте — предметы. Кладём их в центры клеток.
    for (int cy = 0; cy < map_.height(); ++cy) {
        for (int cx = 0; cx < map_.width(); ++cx) {
            const char c = map_.at(cx, cy);
            if (c == 'h' || c == 'a' || c == 'X') {
                const auto kind = c == 'h'   ? ItemKind::Medkit
                                  : c == 'a' ? ItemKind::Clip
                                             : ItemKind::Exit;
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
    seen_.assign(static_cast<std::size_t>(map_.width() *
                                          map_.height()), 0);
}

void Game::update(const Input& in, double dt) {
    if (dead_) {
        // Мёртвый игрок не ходит и не стреляет. Мир живёт дальше.
        deadTime_ += dt;
        if (in.confirm && deadTime_ > 1.0) {
            restart();
            return;
        }
    } else {
        updatePlayer(in, dt);
        time_ += dt;
        lookAround();
    }
    updateDoors(dt);
    if (!dead_) {
        pickUpItems();
    }
    // Поле расстояний до игрока — не каждый кадр: игрок за 0,25 с
    // уходит меньше чем на клетку, а BFS по всей карте не бесплатен.
    flowTimer_ -= dt;
    if (flowTimer_ <= 0.0) {
        const Cell me{static_cast<int>(std::floor(x_)),
                      static_cast<int>(std::floor(y_))};
        flow_ = distanceField(map_, me);
        flowTimer_ = kFlowPeriod;
    }
    if (noise_) {
        wakeByNoise();
        noise_ = false;
    }
    updateMonsters(dt);
    updateProjectiles(dt);
    for (Fx& f : fx_) {
        f.t += dt;
    }
    std::erase_if(fx_, [](const Fx& f) { return f.t > kFxLife; });
    messageTime_ = std::max(0.0, messageTime_ - dt);
    if (messageTime_ == 0.0) {
        message_.clear();
    }
    hurt_ = std::max(0.0, hurt_ - dt * 1.5);   // вспышка гаснет
}

void Game::updatePlayer(const Input& in, double dt) {
    if (in.map) {
        showMap_ = !showMap_;
    }
    if (in.use) {
        useDoor();
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

bool Game::occupied(int cx, int cy) const {
    if (touches(cx, cy)) {
        return true;
    }
    for (const Monster& m : monsters_) {
        const int mx = static_cast<int>(std::floor(m.x));
        const int my = static_cast<int>(std::floor(m.y));
        if (!m.dead() && mx == cx && my == cy) {
            return true;
        }
    }
    return false;
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
        openDoor(*door, hit.cellX, hit.cellY);
    }
}

void Game::openDoor(Door& door, int cx, int cy) {
    if (door.move <= 0 && door.open < 1.0) {  // ещё не ехала
        sound(SoundId::Door, cx + 0.5, cy + 0.5);
    }
    door.move = 1;
}

int Game::itemsTaken() const {
    return static_cast<int>(std::ranges::count_if(
        items_, [](const Item& i) { return i.taken; }));
}

int Game::itemsTotal() const {
    return static_cast<int>(std::ranges::count_if(
        items_,
        [](const Item& i) { return i.kind != ItemKind::Exit; }));
}

bool Game::seen(int cx, int cy) const {
    const int w = map_.width();
    if (cx < 0 || cy < 0 || cx >= w || cy >= map_.height()) {
        return false;
    }
    return seen_[static_cast<std::size_t>(cy * w + cx)] != 0;
}

void Game::lookAround() {
    // Веер лучей по полю зрения: всё, что попало в луч, — видели.
    constexpr int kRays = 48;
    const int w = map_.width();
    const double dirX = std::cos(angle_);
    const double dirY = std::sin(angle_);
    for (int i = 0; i < kRays; ++i) {
        const double cam = 2.0 * i / (kRays - 1) - 1.0;
        const double rx = dirX - dirY * kPlane * cam;
        const double ry = dirY + dirX * kPlane * cam;
        const RayHit hit = castRay(map_, x_, y_, rx, ry);
        const auto mark = [&](int cx, int cy) {
            seen_[static_cast<std::size_t>(cy * w + cx)] = 1;
        };
        // Пол вдоль луча — шагами по полклетки, и стена в конце.
        for (double t = 0.0; t < hit.dist; t += 0.5) {
            mark(static_cast<int>(std::floor(x_ + rx * t)),
                 static_cast<int>(std::floor(y_ + ry * t)));
        }
        mark(hit.cellX, hit.cellY);
    }
}

void Game::sound(SoundId id, double x, double y) {
    const double dist = std::hypot(x - x_, y - y_);
    const double volume = 1.0 - dist / kEarshot;   // линейно в ноль
    if (volume > 0.0) {
        sounds_.push_back({id, static_cast<float>(volume)});
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
                if (occupied(cx, cy)) {
                    openDoor(*d, cx, cy);            // не прищемить!
                    continue;
                }
                d->open = std::max(0.0, d->open - kDoorSpeed * dt);
                if (d->open <= 0.0) {
                    d->move = 0;
                }
            } else if (d->open >= 1.0) {          // стоит открытой
                d->wait -= dt;
                if (d->wait <= 0.0 && !occupied(cx, cy)) {
                    d->move = -1;
                    sound(SoundId::Door, cx + 0.5, cy + 0.5);
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
        if (item.kind == ItemKind::Exit) {
            levelDone_ = true;      // дошёл: дальше решает App
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
        sound(SoundId::Pickup, x_, y_);
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
    noise_ = true;                           // все слышали выстрел
    sound(weapon_ == Weapon::Pistol ? SoundId::Pistol
                                    : SoundId::Shotgun,
          x_, y_);
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
        sound(SoundId::MonsterDeath, target->x, target->y);
        target->state = MonsterState::Dead;
        ++kills_;
    } else {                                 // больно — и зло
        target->state = MonsterState::Pain;
        target->timer = kPain;
        sound(SoundId::MonsterPain, target->x, target->y);
    }
}

void Game::updateProjectiles(double dt) {
    for (Projectile& p : shots_) {
        // Делим путь за кадр на подшаги не длиннее kSubstep:
        // иначе быстрый шар перепрыгнет тонкую преграду.
        const double travel = std::hypot(p.vx, p.vy) * dt;
        const auto parts =
            static_cast<int>(std::ceil(travel / kSubstep));
        const int steps = std::max(1, parts);
        const double h = dt / steps;
        for (int i = 0; i < steps && p.alive; ++i) {
            p.x += p.vx * h;
            p.y += p.vy * h;
            const int cx = static_cast<int>(std::floor(p.x));
            const int cy = static_cast<int>(std::floor(p.y));
            if (map_.isSolid(cx, cy)) {
                p.alive = false;                 // в стену
            } else if (!dead_ && std::hypot(p.x - x_, p.y - y_) <
                                     kHitPlayer) {
                p.alive = false;                 // в игрока
                hurtPlayer(p.damage);
            }
        }
        if (!p.alive) {                          // взрыв чуть раньше
            const double back = 0.1 / std::hypot(p.vx, p.vy);
            fx_.push_back({FxKind::Boom, p.x - p.vx * back,
                           p.y - p.vy * back});
            sound(SoundId::Boom, p.x, p.y);
        }
    }
    std::erase_if(shots_,
                  [](const Projectile& p) { return !p.alive; });
}

void Game::restart() {
    // Всё как в начале уровня: собираем игру заново из исходной
    // карты. Временный объект строится раньше, чем затирается *this.
    *this = Game{initial_};
}

void Game::render(Canvas& canvas) const {
    if (showMap_) {
        drawTopView(canvas, *this);
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
    for (const Projectile& p : shots_) {
        const Image* img = &fxImage(FxKind::Fireball);
        sprites.push_back({p.x, p.y, img, 0.0, 1.0, true});
    }
    for (const Fx& f : fx_) {                // облачко растёт и тает
        const double speed = f.kind == FxKind::Boom ? 6.0 : 2.0;
        const double grow = 1.0 + f.t * speed;
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
    if (!dead_) {
        const bool firing = flash_ > 0.0;
        drawWeapon(canvas, weapon_, firing, bobX,
                   bobY + recoil - kStatusBar);
    }
    const int cx = canvas.width() / 2;       // перекрестие
    const int cy = canvas.height() / 2;
    canvas.fillRect(cx - 6, cy, 4, 1, rgb(96, 255, 96));
    canvas.fillRect(cx + 3, cy, 4, 1, rgb(96, 255, 96));
    canvas.fillRect(cx, cy - 6, 1, 4, rgb(96, 255, 96));
    canvas.fillRect(cx, cy + 3, 1, 4, rgb(96, 255, 96));

    // Красная пелена: от боли вспыхивает, после смерти — остаётся.
    const double red = dead_ ? 0.5 : std::min(0.55, hurt_ * 0.55);
    if (red > 0.0) {
        const auto s = static_cast<std::uint32_t>(red * 256.0);
        for (int y = 0; y < canvas.height(); ++y) {
            for (int x = 0; x < canvas.width(); ++x) {
                const std::uint32_t c = canvas.at(x, y);
                canvas.put(x, y, shade(c, 256 - s) +
                                     shade(rgb(200, 0, 0), s));
            }
        }
    }
}

void Game::teleport(double x, double y, double angle) {
    x_ = x;
    y_ = y;
    angle_ = angle;
}

} // namespace doom
