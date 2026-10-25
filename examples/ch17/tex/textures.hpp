// textures.hpp — текстуры стен, нарисованные кодом
#pragma once

#include "image.hpp"

namespace doom {

inline constexpr int kTexSize = 64;   // 64 × 64, степень двойки

[[nodiscard]] Image makeStone();
[[nodiscard]] Image makeBricks();
[[nodiscard]] Image makeTech();

// Текстура клетки карты: '1' — камень, '2' — кирпич, '3' — металл.
[[nodiscard]] const Image& wallTexture(char cell);

} // namespace doom
