// ai.cpp — мозги монстров: автомат, видимость, поиск пути
#include "game.hpp"

#include "pathfind.hpp"
#include "raycast.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <random>

namespace doom {

namespace {
constexpr double kSight = 12.0;    // дальше не видят
constexpr double kBody = 0.3;      // радиус монстра для стен
constexpr double kPersonal = 0.7;  // ближе к игроку не подходят
constexpr int kHearing = 10;       // слышат выстрел за 10 шагов
constexpr double kFireball = 6.0;  // скорость шара, клеток в секунду
} // namespace

void Game::updateMonsters(double dt) {
    for (Monster& m : monsters_) {
        m.anim += dt;
        m.cooldown = std::max(0.0, m.cooldown - dt);
        updateMonster(m, dt);
    }
}

void Game::updateMonster(Monster& m, double dt) {
    const MonsterSpec& spec = specOf(m.kind);
    const double dx = x_ - m.x;             // от монстра к игроку
    const double dy = y_ - m.y;
    const double dist = std::hypot(dx, dy);
    switch (m.state) {
    case MonsterState::Idle:
        if (canSee(m)) {
            m.state = MonsterState::Chase;  // заметил!
        }
        break;
    case MonsterState::Chase:
        if (m.cooldown == 0.0 && dist < spec.reach && canSee(m)) {
            m.state = MonsterState::Attack;
            m.timer = spec.windup;
            m.struck = false;
            break;
        }
        if (!canSee(m)) {                   // не видно — по полю
            followFlow(m, spec.speed * dt);
        } else if (dist > kPersonal) {      // видно — прямо к игроку
            const double step = spec.speed * dt / dist;
            tryMove(m.x, m.y, dx * step, dy * step, kBody);
        }
        break;
    case MonsterState::Attack:
        m.timer -= dt;
        // Удар — в середине замаха: у игрока есть миг увернуться.
        if (!m.struck && m.timer <= spec.windup / 2) {
            m.struck = true;
            monsterAttack(m, dist);
        }
        if (m.timer <= 0.0) {
            m.state = MonsterState::Chase;
            m.cooldown = spec.rest;
        }
        break;
    case MonsterState::Pain:
        m.timer -= dt;
        if (m.timer <= 0.0) {
            m.state = MonsterState::Chase; // боль прошла, злость нет
        }
        break;
    case MonsterState::Dead:
        break;
    }
}

bool Game::canSee(const Monster& m) const {
    const double dx = x_ - m.x;
    const double dy = y_ - m.y;
    if (dx * dx + dy * dy > kSight * kSight) {
        return false;
    }
    // Луч ровно до игрока: стена ближе единицы — не видно.
    return castRay(map_, m.x, m.y, dx, dy).dist >= 1.0;
}

void Game::monsterAttack(const Monster& m, double dist) {
    const MonsterSpec& spec = specOf(m.kind);
    std::uniform_int_distribution<int> damage(spec.minDamage,
                                              spec.maxDamage);
    if (m.kind == MonsterKind::Imp) {
        // Имп бросает огненный шар туда, где игрок сейчас. Шар летит
        // по-настоящему: от него можно увернуться.
        const double ux = (x_ - m.x) / dist;
        const double uy = (y_ - m.y) / dist;
        shots_.push_back({m.x + ux * 0.4, m.y + uy * 0.4,
                          ux * kFireball, uy * kFireball,
                          damage(rng_)});
        return;
    }
    // Зомби стреляет: чем дальше игрок, тем чаще мимо.
    const double chance = std::clamp(1.0 - dist / 10.0, 0.2, 0.9);
    std::uniform_real_distribution<double> roll(0.0, 1.0);
    if (canSee(m) && roll(rng_) <= chance) {
        hurtPlayer(damage(rng_));
    }
}

void Game::followFlow(Monster& m, double step) {
    const int w = map_.width();
    const auto at = [&](int x, int y) {
        return flow_[static_cast<std::size_t>(y * w + x)];
    };
    const int cx = static_cast<int>(std::floor(m.x));
    const int cy = static_cast<int>(std::floor(m.y));
    // Из четырёх соседей — тот, что на шаг ближе к игроку.
    constexpr std::array<Cell, 4> kSteps{
        {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
    Cell next{cx, cy};
    int best = at(cx, cy);
    for (const Cell s : kSteps) {
        const int d = at(cx + s.x, cy + s.y);
        if (d != kUnreached && (best == kUnreached || d < best)) {
            best = d;
            next = {cx + s.x, cy + s.y};
        }
    }
    if (next.x == cx && next.y == cy) {
        return;                             // пути нет или мы у цели
    }
    if (Door* door = map_.door(next.x, next.y)) {
        if (door->open < 0.9) {             // закрыто — открываем
            door->move = 1;                 // и ждём
            return;
        }
    }
    // Идём к центру следующей клетки. Но сначала встаём на середину
    // своей поперёк пути — иначе плечом зацепимся за угол стены.
    double tx = next.x + 0.5 - m.x;
    double ty = next.y + 0.5 - m.y;
    if (next.x == cx && std::abs(cx + 0.5 - m.x) > 0.05) {
        ty = 0.0;                           // сначала выровнять x
    } else if (next.y == cy && std::abs(cy + 0.5 - m.y) > 0.05) {
        tx = 0.0;                           // сначала выровнять y
    }
    const double len = std::hypot(tx, ty);
    tryMove(m.x, m.y, tx / len * step, ty / len * step, kBody);
}

void Game::wakeByNoise() {
    const int w = map_.width();
    for (Monster& m : monsters_) {
        if (m.state != MonsterState::Idle) {
            continue;
        }
        const int cx = static_cast<int>(std::floor(m.x));
        const int cy = static_cast<int>(std::floor(m.y));
        const int d = flow_[static_cast<std::size_t>(cy * w + cx)];
        if (d != kUnreached && d <= kHearing) {
            m.state = MonsterState::Chase;  // услышал — идёт искать
        }
    }
}

void Game::hurtPlayer(int damage) {
    health_ = std::max(0, health_ - damage);
    hurt_ = std::min(1.0, hurt_ + damage / 20.0); // больнее — ярче
    if (health_ == 0 && !dead_) {
        dead_ = true;
        deadTime_ = 0.0;
    }
}

} // namespace doom
