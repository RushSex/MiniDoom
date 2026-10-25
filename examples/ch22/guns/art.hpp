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
enum class FxKind : std::uint8_t { Puff, Blood };

struct Fx {
    FxKind kind;
    double x = 0.0;
    double y = 0.0;
    double t = 0.0;          // сколько уже живёт, секунды
};

[[nodiscard]] const Image& fxImage(FxKind kind);

} // namespace doom
