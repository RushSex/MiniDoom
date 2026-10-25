#include <iostream>

// part от total в процентах, округление честное; 0 из 0 — 100%.
[[nodiscard]] int percent(int part, int total) {
    if (total == 0) {
        return 100;
    }
    return (part * 200 + total) / (2 * total);
}

int main() {
    std::cout << percent(2, 3) << "% " << percent(4, 9) << "% "
              << percent(0, 0) << "%\n";
}
