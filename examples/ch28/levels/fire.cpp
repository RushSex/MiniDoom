// fire.cpp — каждый пиксель берёт жар снизу, чуть остывший
#include "fire.hpp"

#include <algorithm>
#include <array>

namespace doom {

namespace {
// 37 цветов: чёрный → тёмно-красный → оранжевый → жёлтый → белый.
const std::array<std::uint32_t, 37>& palette() {
    static const auto colors = [] {
        constexpr std::array<std::uint32_t, 5> keys{
            0x070707, 0x8F2707, 0xDF5707, 0xCFAF2F, 0xFFFFFF};
        std::array<std::uint32_t, 37> out{};
        for (std::size_t i = 0; i < out.size(); ++i) {
            const double t = static_cast<double>(i) / 36.0 * 4.0;
            const auto k = std::min<std::size_t>(
                static_cast<std::size_t>(t), 3);
            const double f = t - static_cast<double>(k);
            const auto mix = [&](int shift) {
                const auto a = static_cast<double>(
                    (keys[k] >> shift) & 0xFF);
                const auto b = static_cast<double>(
                    (keys[k + 1] >> shift) & 0xFF);
                return static_cast<std::uint32_t>(a + (b - a) * f)
                       << shift;
            };
            out[i] = mix(16) | mix(8) | mix(0);
        }
        return out;
    }();
    return colors;
}
} // namespace

Fire::Fire(int width, int height)
    : width_(width), height_(height),
      heat_(static_cast<std::size_t>(width * height), 0) {
    for (int x = 0; x < width_; ++x) {          // нижний ряд горит
        heat_[static_cast<std::size_t>((height_ - 1) * width_ + x)] =
            kHot;
    }
}

void Fire::step(std::mt19937& rng) {
    for (int y = 1; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const int src = y * width_ + x;
            const auto r = static_cast<int>(rng() & 3);   // 0…3
            // Строкой выше и чуть вбок — ветер; остыть на 0 или 1.
            const int dst = src - width_ - r + 1;
            if (dst < 0) {
                continue;
            }
            const auto h = heat_[static_cast<std::size_t>(src)];
            const int cool = r & 1;
            heat_[static_cast<std::size_t>(dst)] =
                static_cast<std::uint8_t>(h > cool ? h - cool : 0);
        }
    }
}

void Fire::draw(Canvas& canvas, int scale) const {
    const auto& pal = palette();
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const auto i = static_cast<std::size_t>(y * width_ + x);
            canvas.fillRect(x * scale, y * scale, scale, scale,
                            pal[heat_[i]]);
        }
    }
}

} // namespace doom
