// monsters.hpp — монстры: кто, где и как выглядят
#pragma once

#include "image.hpp"

#include <cstdint>

namespace doom {

enum class MonsterKind : std::uint8_t { Imp, Zombie };
enum class Pose : std::uint8_t { Walk1, Walk2, Pain, Dead };

struct Monster {
    MonsterKind kind;
    double x = 0.0;
    double y = 0.0;
    double anim = 0.0;       // время анимации, секунды
    int hp = 0;              // здоровье; 0 и меньше — мёртв
    double pain = 0.0;       // сколько ещё корчиться от попадания
    [[nodiscard]] bool dead() const { return hp <= 0; }
};

// Сколько здоровья у монстра этого вида в начале.
[[nodiscard]] int startHp(MonsterKind kind);

[[nodiscard]] Image makeImp(Pose pose);
[[nodiscard]] Image makeZombie(Pose pose);
[[nodiscard]] Image makeCorpse(MonsterKind kind);
// Кадр для монстра этого вида в этой позе; строится один раз.
[[nodiscard]] const Image& monsterImage(MonsterKind kind, Pose pose);

} // namespace doom
