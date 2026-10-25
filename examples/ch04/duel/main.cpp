// main.cpp — сам бой читается как сценарий
#include "combat.hpp"

#include <iostream>
#include <random>

int main() {
    std::mt19937 rng(2);
    int playerHealth = 100;
    int impHealth = 60;
    int medkits = 1;
    int turn = 0;

    std::cout << "An imp appears!\n";
    while (playerHealth > 0 && impHealth > 0) {
        ++turn;
        doom::printStatus(turn, playerHealth, impHealth, medkits);
        const char action = doom::readAction();

        if (action == '1') {
            const int hit = doom::shoot(impHealth, rng);
            std::cout << "You hit for " << hit << ". ";
        } else if (action == '2') {
            if (doom::useMedkit(playerHealth, medkits)) {
                std::cout << "Medkit used. ";
            } else {
                std::cout << "No medkits! ";
            }
        } else {
            std::cout << "You run like a coward. Game over.\n";
            return 0;
        }

        if (impHealth <= 0) {
            std::cout << '\n';
            break;
        }
        const int dmg = doom::impAttack(playerHealth, rng);
        if (dmg > 0) {
            std::cout << "Fireball: -" << dmg << " HP.\n";
        } else {
            std::cout << "Fireball misses.\n";
        }
    }

    if (playerHealth > 0) {
        std::cout << "Imp dead! Won in " << turn << " turns!\n";
    } else {
        std::cout << "You died. The imp dances on your corpse.\n";
    }
}
