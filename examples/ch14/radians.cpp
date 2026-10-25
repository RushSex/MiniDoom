#include <cmath>
#include <iostream>
#include <numbers>

int main() {
    // Хотели синус 90 градусов, а это должна быть единица.
    std::cout << "sin(90)       = " << std::sin(90.0) << '\n';

    // Функции из <cmath> считают в радианах: 180° — это π.
    const double rad = 90.0 * std::numbers::pi / 180.0;
    std::cout << "sin(90 deg)   = " << std::sin(rad) << '\n';
    std::cout << "90 deg in rad = " << rad << '\n';
}
