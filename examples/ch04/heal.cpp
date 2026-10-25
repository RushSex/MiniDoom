#include <iostream>

// Возвращает здоровье после аптечки, но не больше 100.
[[nodiscard]] int heal(int health, int amount) {
    const int result = health + amount;
    if (result > 100) {
        return 100;
    }
    return result;
}

int main() {
    int health = 85;
    health = heal(health, 25);
    std::cout << "health: " << health << '\n';
    std::cout << "from 40: " << heal(40, 25) << '\n';
}
