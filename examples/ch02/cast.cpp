#include <iostream>

int main() {
    int kills = 3;
    int total = 14;

    std::cout << "wrong: " << kills / total * 100 << "%\n";
    std::cout << "int:   " << kills * 100 / total << "%\n";
    const double exact = static_cast<double>(kills) / total * 100;
    std::cout << "exact: " << exact << "%\n";

    double time = 83.9;                      // секунд на уровне
    int seconds = static_cast<int>(time);    // дробь отрезается
    std::cout << "seconds: " << seconds << '\n';
}
