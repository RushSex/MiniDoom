#include <format>
#include <iostream>

constexpr int kImpHealth = 60;
constexpr int kZombieHealth = 20;
constexpr double kPistolAvg = (5 + 15) / 2.0;

int main() {
    const int imps = 7;
    const int zombies = 7;
    const int hp = imps * kImpHealth + zombies * kZombieHealth;
    const double bullets = hp / kPistolAvg;
    std::cout << std::format("Total monster HP: {}\n", hp);
    std::cout << std::format("Bullets on avg: {:.0f}\n", bullets);
}
