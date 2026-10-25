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

} // namespace doom
