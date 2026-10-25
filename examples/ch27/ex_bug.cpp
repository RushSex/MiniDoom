#include <cstdint>
#include <iostream>
#include <random>
#include <vector>

// Огонь 8 × 4, как в fire.cpp, только проверку кто-то «упростил».
int main() {
    constexpr int w = 8;
    constexpr int h = 4;
    std::vector<std::uint8_t> heat(w * h, 0);
    for (int x = 0; x < w; ++x) {
        heat[static_cast<std::size_t>((h - 1) * w + x)] = 36;
    }
    std::mt19937 rng{1};
    for (int y = 1; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            const int src = y * w + x;
            const int r = static_cast<int>(rng() & 3);
            const int dst = src - w - r + 1;
            const auto from = heat[static_cast<std::size_t>(src)];
            heat[static_cast<std::size_t>(dst)] =
                static_cast<std::uint8_t>(from - (r & 1));
        }
    }
    std::cout << "top row: " << int{heat[0]} << '\n';
}
