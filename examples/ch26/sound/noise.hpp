// noise.hpp — процедурный шум: «случайность», которая повторяется
#pragma once

#include <cstdint>

namespace doom {

// Число от 0 до 1, которое зависит только от (x, y, seed).
[[nodiscard]] double hash01(int x, int y, std::uint32_t seed);

// Плавный шум: случайные значения в целых точках, между ними —
// гладкий переход. period — через сколько клеток узор повторится.
[[nodiscard]] double valueNoise(double x, double y, int period,
                                std::uint32_t seed);

// Сумма октав: крупные пятна + мелкие + ещё мельче. Результат 0…1.
[[nodiscard]] double fbm(double x, double y, int octaves, int period,
                         std::uint32_t seed);

} // namespace doom
