#include <iostream>

struct Player {
    int health = 100;
    int ammo = 50;
};

void printStatus(const Player& p) {      // только читает — const&
    std::cout << "HP " << p.health << ", ammo " << p.ammo << '\n';
}

void pickupMedkit(Player& p) {           // меняет — просто &
    p.health += 25;
    if (p.health > 100) {
        p.health = 100;
    }
}

int main() {
    Player hero{.health = 40};
    printStatus(hero);
    pickupMedkit(hero);
    printStatus(hero);
}
