#include <array>
#include <format>
#include <iostream>
#include <string_view>

struct Weapon {
    std::string_view name;
    int minDmg = 0;
    int maxDmg = 0;
    int pellets = 1;
};

constexpr std::array<Weapon, 3> kWeapons{{
    {"Fist", 2, 20, 1},
    {"Pistol", 5, 15, 1},
    {"Shotgun", 5, 15, 7},
}};

int main() {
    constexpr auto kRow = "{:<8} {:>2}-{:<2} x{}  avg {:>5.1f}\n";
    for (const Weapon& w : kWeapons) {
        const double avg = (w.minDmg + w.maxDmg) * w.pellets / 2.0;
        std::cout << std::format(kRow, w.name, w.minDmg, w.maxDmg,
                                 w.pellets, avg);
    }
}
