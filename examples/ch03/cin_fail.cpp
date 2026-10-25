#include <iostream>

int main() {
    int ammo = 50;
    std::cout << "How much ammo? ";
    std::cin >> ammo;
    if (!std::cin) {
        std::cout << "That's not a number! ammo = " << ammo << '\n';
    } else {
        std::cout << "OK, ammo = " << ammo << '\n';
    }
}
