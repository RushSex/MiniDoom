// noise.cpp — хеш, гладкий шум и fbm
#include "noise.hpp"

#include <cmath>

namespace doom {

double hash01(int x, int y, std::uint32_t seed) {
    // Перемешиваем биты: близкие входы дают совсем разные выходы.
    std::uint32_t h = static_cast<std::uint32_t>(x) * 374761393u +
                      static_cast<std::uint32_t>(y) * 668265263u +
                      seed * 2246822519u;
    h = (h ^ (h >> 13)) * 1274126177u;
    h ^= h >> 16;
    return h / 4294967295.0;                 // 0 … 1
}

double valueNoise(double x, double y, int period,
                  std::uint32_t seed) {
    const int ix = static_cast<int>(std::floor(x));
    const int iy = static_cast<int>(std::floor(y));
    // Сглаживаем дробную часть: у краёв клетки переход плавный.
    const auto smooth = [](double t) {
        return t * t * (3.0 - 2.0 * t);
    };
    const double fx = smooth(x - ix);
    const double fy = smooth(y - iy);
    // Номер точки по модулю period — так узор бесшовно повторяется.
    const auto wrap = [period](int v) {
        return ((v % period) + period) % period;
    };
    const auto at = [&](int dx, int dy) {
        return hash01(wrap(ix + dx), wrap(iy + dy), seed);
    };
    const double top = at(0, 0) + (at(1, 0) - at(0, 0)) * fx;
    const double bottom = at(0, 1) + (at(1, 1) - at(0, 1)) * fx;
    return top + (bottom - top) * fy;
}

double fbm(double x, double y, int octaves, int period,
           std::uint32_t seed) {
    double sum = 0.0;
    double amp = 0.5;               // вклад октавы: 1/2, 1/4, 1/8…
    double total = 0.0;
    for (int i = 0; i < octaves; ++i) {
        const auto octaveSeed = seed + static_cast<std::uint32_t>(i);
        sum += amp * valueNoise(x, y, period, octaveSeed);
        total += amp;
        x *= 2.0;                   // следующая октава вдвое мельче
        y *= 2.0;
        period *= 2;
        amp *= 0.5;
    }
    return sum / total;
}

} // namespace doom
