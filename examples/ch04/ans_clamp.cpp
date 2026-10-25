#include <algorithm>
#include <iostream>

[[nodiscard]] int clampInt(int value, int lo, int hi) {
    if (value < lo) {
        return lo;
    }
    if (value > hi) {
        return hi;
    }
    return value;
}

int main() {
    std::cout << clampInt(130, 0, 100) << ' '
              << clampInt(-5, 0, 100) << ' '
              << clampInt(42, 0, 100) << '\n';
    // то же самое из стандартной библиотеки:
    std::cout << std::clamp(130, 0, 100) << '\n';
}
