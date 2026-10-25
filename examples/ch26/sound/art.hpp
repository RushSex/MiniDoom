// art.hpp — картинки предметов, нарисованные кодом
#pragma once

#include "image.hpp"
#include "items.hpp"

namespace doom {

// Непрозрачный цвет: старший байт — альфа 255.
[[nodiscard]] constexpr std::uint32_t opaque(std::uint32_t rgb) {
    return 0xFF000000u | rgb;
}

[[nodiscard]] Image makeMedkit();
[[nodiscard]] Image makeClip();
[[nodiscard]] const Image& itemImage(ItemKind kind);

// Следы попаданий: облачко пыли от стены и брызги крови.
enum class FxKind : std::uint8_t { Puff, Blood, Boom, Fireball };

struct Fx {
    FxKind kind;
    double x = 0.0;
    double y = 0.0;
    double t = 0.0;          // сколько уже живёт, секунды
};

[[nodiscard]] const Image& fxImage(FxKind kind);

// Огненный шар импа: летит, пока во что-нибудь не врежется.
struct Projectile {
    double x = 0.0;
    double y = 0.0;
    double vx = 0.0;         // скорость, клеток в секунду
    double vy = 0.0;
    int damage = 0;          // сколько снимет, если попадёт
    bool alive = true;
};

} // namespace doom
