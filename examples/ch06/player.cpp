#include <iostream>

struct Player {
    int x = 1;          // значения по умолчанию
    int y = 1;
    int health = 100;
    int ammo = 50;
};

int main() {
    Player hero;                         // всё по умолчанию
    hero.health -= 30;                   // точка — доступ к полю
    std::cout << "hero: (" << hero.x << ',' << hero.y << ") HP "
              << hero.health << ", ammo " << hero.ammo << '\n';

    Player clone{.x = 5, .y = 3};        // только нужные поля
    std::cout << "clone: (" << clone.x << ',' << clone.y << ") HP "
              << clone.health << '\n';
}
