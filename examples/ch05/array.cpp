#include <array>
#include <iostream>

int main() {
    // Урон: пистолет, дробовик, кулак
    std::array<int, 3> damage{10, 70, 5};

    std::cout << "weapons: " << damage.size() << '\n';
    std::cout << "shotgun: " << damage[1] << '\n';

    damage[2] = 7;                  // прокачали кулаки
    int total = 0;
    for (int d : damage) {          // перебираем все элементы
        total += d;
    }
    std::cout << "total:   " << total << '\n';
}
