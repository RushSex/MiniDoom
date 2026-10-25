// monsters.hpp — монстры: кто, где и как выглядят
#pragma once

#include "image.hpp"

#include <cstdint>

namespace doom {

enum class MonsterKind : std::uint8_t { Imp, Zombie };
enum class Pose : std::uint8_t { Walk1, Walk2 };

struct Monster {
    MonsterKind kind;
    double x = 0.0;
    double y = 0.0;
    double anim = 0.0;       // время анимации, секунды
};

[[nodiscard]] Image makeImp(Pose pose);
[[nodiscard]] Image makeZombie(Pose pose);
// Кадр для монстра этого вида в этой позе; строится один раз.
[[nodiscard]] const Image& monsterImage(MonsterKind kind, Pose pose);

} // namespace doom
