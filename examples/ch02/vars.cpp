#include <iostream>

int main() {
    int health = 100;   // здоровье игрока
    int ammo = 50;      // патроны
    double speed = 3.4; // клеток в секунду

    std::cout << "HEALTH: " << health << '\n';
    std::cout << "AMMO:   " << ammo << '\n';
    std::cout << "SPEED:  " << speed << '\n';

    ammo = ammo - 1;    // выстрелили
    health -= 15;       // в нас попали
    std::cout << "after: " << health << " HP, " << ammo << " ammo\n";
}
