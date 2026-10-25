#include <iostream>

void reload(int ammo) {
    ammo += 10;
    std::cout << "reload: " << ammo << '\n';
}

void pickup(int& ammo, int amount) {
    ammo += amount;
}

int doubled(int ammo) {
    return ammo * 2;
}

int main() {
    int ammo = 5;
    reload(ammo);
    pickup(ammo, 20);
    std::cout << ammo << ' ' << doubled(ammo) << ' ' << ammo << '\n';
}
