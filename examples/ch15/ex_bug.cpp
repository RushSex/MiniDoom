#include <cmath>
#include <iostream>

// Расстояние до первой линии сетки по x — с ошибкой.
double firstLineX(double ox, double dx) {
    const int cx = static_cast<int>(std::floor(ox));
    return (cx + 1.0 - ox) * std::abs(1.0 / dx);
}

int main() {
    std::cout << firstLineX(1.25, 1.0) << ' '     // вправо
              << firstLineX(1.25, -1.0) << '\n';  // влево
}
