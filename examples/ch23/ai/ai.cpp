// ai.cpp — мозги монстров: конечный автомат и линия видимости
#include "game.hpp"

#include "raycast.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace doom {

namespace {
constexpr double kSight = 12.0;    // дальше не видят
constexpr double kBody = 0.3;      // радиус монстра для стен
constexpr double kPersonal = 0.7;  // ближе к игроку не подходят
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
        if (dist > kPersonal) {             // идём прямо к игроку
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
    if (m.kind == MonsterKind::Imp) {
        if (dist > spec.reach + 0.2) {
            return;                         // игрок отскочил
        }
    } else {
        // Зомби стреляет: чем дальше игрок, тем чаще мимо.
        const double chance =
            std::clamp(1.0 - dist / 10.0, 0.2, 0.9);
        std::uniform_real_distribution<double> roll(0.0, 1.0);
        if (!canSee(m) || roll(rng_) > chance) {
            return;
        }
    }
    std::uniform_int_distribution<int> damage(spec.minDamage,
                                              spec.maxDamage);
    hurtPlayer(damage(rng_));
}

void Game::hurtPlayer(int damage) {
    // Пока игрок бессмертен: ниже нуля не опускаемся.
    // Смерть и её последствия — в главе 25.
    health_ = std::max(0, health_ - damage);
}

} // namespace doom
