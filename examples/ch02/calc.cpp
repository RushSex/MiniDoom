// calc.cpp — сколько выстрелов нужно, чтобы уложить импа
#include <format>
#include <iostream>

constexpr int kImpHealth = 60;
constexpr int kPistolMin = 5;   // урон пистолета: от 5
constexpr int kPistolMax = 15;  //               до 15
constexpr int kPellets = 7;     // дробинок в патроне

int main() {
    // Деление с округлением вверх в целых: (a + b - 1) / b.
    const int best = (kImpHealth + kPistolMax - 1) / kPistolMax;
    const int worst = (kImpHealth + kPistolMin - 1) / kPistolMin;
    const double average = (kPistolMin + kPistolMax) / 2.0;
    const double shotsAvg = kImpHealth / average;
    const double shotgunAvg = average * kPellets;

    constexpr auto kInt = "{:<22}{:>6}\n";      // шаблон для целых
    constexpr auto kReal = "{:<22}{:>6.1f}\n";  // и для дробных
    std::cout << std::format(kInt, "Imp health:", kImpHealth);
    std::cout << std::format(kInt, "Pistol, lucky:", best);
    std::cout << std::format(kInt, "Pistol, unlucky:", worst);
    std::cout << std::format(kReal, "Pistol, on average:", shotsAvg);
    std::cout << std::format(kReal, "Shotgun average:", shotgunAvg);
}
