// monsters.hpp — монстры: кто, где, в каком состоянии, как выглядят
#pragma once

#include "image.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace doom {

enum class MonsterKind : std::uint8_t { Imp, Zombie };
enum class Pose : std::uint8_t { Walk1, Walk2, Pain, Dead, Attack };

// Состояние «мозгов»: что монстр делает прямо сейчас.
enum class MonsterState : std::uint8_t {
    Idle,      // стоит, ждёт, пока увидит игрока
    Chase,     // идёт к игроку
    Attack,    // замахнулся или целится
    Pain,      // получил пулю и корчится
    Dead,
};

struct Monster {
    MonsterKind kind;
    double x = 0.0;
    double y = 0.0;
    double anim = 0.0;       // время анимации, секунды
    int hp = 0;
    MonsterState state = MonsterState::Idle;
    double timer = 0.0;      // сколько ещё длится Attack или Pain
    double cooldown = 0.0;   // пауза до следующей атаки
    bool struck = false;     // удар этой атаки уже нанесён
    [[nodiscard]] bool dead() const {
        return state == MonsterState::Dead;
    }
};

struct MonsterSpec {
    int hp;
    double speed;            // клеток в секунду
    double reach;            // с какого расстояния атакует
    double windup;           // сколько длится атака, секунды
    int minDamage;
    int maxDamage;
    double rest;             // пауза после атаки
};

inline constexpr std::array<MonsterSpec, 2> kMonsters{{
    {60, 1.6, 1.0, 0.5, 6, 12, 1.0},    // имп: когти вблизи
    {20, 1.2, 8.0, 0.6, 3, 10, 1.5},    // зомби: стреляет издалека
}};

[[nodiscard]] constexpr const MonsterSpec& specOf(MonsterKind k) {
    return kMonsters[static_cast<std::size_t>(k)];
}

[[nodiscard]] Image makeImp(Pose pose);
[[nodiscard]] Image makeZombie(Pose pose);
[[nodiscard]] Image makeCorpse(MonsterKind kind);
// Кадр для монстра этого вида в этой позе; строится один раз.
[[nodiscard]] const Image& monsterImage(MonsterKind kind, Pose pose);

} // namespace doom
