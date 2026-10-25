// fire.hpp — огонь с титульника PSX Doom: клеточный автомат
#pragma once

#include "canvas.hpp"

#include <cstdint>
#include <random>
#include <vector>

namespace doom {

class Fire {
public:
    Fire(int width, int height);

    void step(std::mt19937& rng);   // один шаг: огонь ползёт вверх
    void draw(Canvas& canvas, int scale) const;

private:
    static constexpr std::uint8_t kHot = 36;  // жар у самого низа

    int width_;
    int height_;
    std::vector<std::uint8_t> heat_;    // 0 — холодно, 36 — бело
};

} // namespace doom
