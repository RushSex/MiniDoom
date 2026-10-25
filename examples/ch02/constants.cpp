#include <iostream>

constexpr int kImpHealth = 60;     // не меняется никогда
constexpr int kPistolMin = 5;
constexpr int kPistolMax = 15;

int main() {
    auto ammo = 50;                // auto: тип выведется из 50 — int
    const auto half = ammo / 2;
    std::cout << "imp: " << kImpHealth << ", half: " << half << '\n';
    // kImpHealth = 100;           // раскомментируй — не соберётся
    std::cout << "dmg: " << kPistolMin << ".." << kPistolMax << '\n';
}
