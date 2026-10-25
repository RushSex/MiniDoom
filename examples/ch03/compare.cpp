#include <iostream>

int main() {
    int health = 15;
    int ammo = 0;

    std::cout << std::boolalpha;           // true/false вместо 1/0
    std::cout << (health < 25) << '\n';    // меньше
    std::cout << (ammo == 0) << '\n';      // равно
    std::cout << (ammo != 0) << '\n';      // не равно
    std::cout << (health >= 100) << '\n';  // больше или равно
}
