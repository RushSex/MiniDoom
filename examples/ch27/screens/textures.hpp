// textures.hpp — текстуры стен, нарисованные кодом
#pragma once

#include "image.hpp"

namespace doom {

inline constexpr int kTexSize = 64;   // 64 × 64, степень двойки

[[nodiscard]] Image makeStone();
[[nodiscard]] Image makeBricks();
[[nodiscard]] Image makeTech();
[[nodiscard]] Image makeDoor();
[[nodiscard]] Image makeFloor();
[[nodiscard]] Image makeCeiling();

// Текстура клетки: '1' — камень, '2' — кирпич, '3' — металл,
// 'D' — дверь.
[[nodiscard]] const Image& wallTexture(char cell);
[[nodiscard]] const Image& floorTexture();
[[nodiscard]] const Image& ceilingTexture();

} // namespace doom
