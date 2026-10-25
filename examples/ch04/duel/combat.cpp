// combat.cpp — реализация: КАК именно всё работает
#include "combat.hpp"

#include <iostream>

namespace doom {

void printStatus(int turn, int playerHealth, int impHealth,
                 int medkits) {
    std::cout << "[T" << turn << " HP " << playerHealth
              << " | imp " << impHealth << " | med " << medkits
              << "] 1-shoot 2-heal 3-run: ";
}

char readAction() {
    char action = '3';
    if (!(std::cin >> action)) {
        return '3';
    }
    return action;
}

int shoot(int& impHealth, std::mt19937& rng) {
    std::uniform_int_distribution<int> pistol(5, 15);
    const int dmg = pistol(rng);
    impHealth -= dmg;
    return dmg;
}

bool useMedkit(int& playerHealth, int& medkits) {
    if (medkits == 0) {
        return false;
    }
    --medkits;
    playerHealth += 25;
    if (playerHealth > 100) {
        playerHealth = 100;
    }
    return true;
}

int impAttack(int& playerHealth, std::mt19937& rng) {
    std::uniform_int_distribution<int> percent(1, 100);
    std::uniform_int_distribution<int> fireball(3, 24);
    if (percent(rng) > 60) {
        return 0;                    // промах
    }
    const int dmg = fireball(rng);
    playerHealth -= dmg;
    return dmg;
}

} // namespace doom
