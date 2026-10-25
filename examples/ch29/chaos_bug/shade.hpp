// shade.hpp — свет: чем дальше, тем темнее
#pragma once

#include <algorithm>
#include <cstdint>

namespace doom {

// Яркость s из 256: 256 — как есть, 128 — вдвое темнее.
// R и B умножаются за одно умножение: между ними 8 пустых бит,
// и произведение одного канала не залезает в другой.
[[nodiscard]] inline std::uint32_t shade(std::uint32_t c,
                                         std::uint32_t s) {
    const std::uint32_t rb = ((c & 0xFF00FF) * s >> 8) & 0xFF00FF;
    const std::uint32_t g = ((c & 0x00FF00) * s >> 8) & 0x00FF00;
    return rb | g;
}

// Сколько света доходит с расстояния dist клеток: от 256 вблизи
// до 16 вдали. Как в Doom: коридоры уходят в темноту.
[[nodiscard]] inline std::uint32_t lightAt(double dist) {
    const double k = std::clamp(1.05 - 0.12 * dist, 0.06, 1.0);
    return static_cast<std::uint32_t>(k * 256.0);
}

} // namespace doom
