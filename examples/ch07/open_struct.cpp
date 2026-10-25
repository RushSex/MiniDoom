#include <iostream>

struct Player {
    int health = 100;
};

int main() {
    Player hero;
    hero.health -= 150;              // урон больше здоровья
    std::cout << "health: " << hero.health << '\n';
    hero.health += 500;              // «аптечка» из кривого кода
    std::cout << "health: " << hero.health << '\n';
}
