// duel.cpp — пошаговая дуэль с импом
#include <iostream>
#include <random>

int main() {
    std::mt19937 rng(2);    // зерно фиксировано: бой повторяется
    std::uniform_int_distribution<int> pistol(5, 15);
    std::uniform_int_distribution<int> fireball(3, 24);
    std::uniform_int_distribution<int> percent(1, 100);

    int playerHealth = 100;
    int impHealth = 60;
    int medkits = 1;
    int turn = 0;

    std::cout << "An imp appears!\n";
    while (playerHealth > 0 && impHealth > 0) {
        ++turn;
        std::cout << "[T" << turn << " HP " << playerHealth
                  << " | imp " << impHealth << " | med " << medkits
                  << "] 1-shoot 2-heal 3-run: ";
        char action = '3';
        if (!(std::cin >> action)) {
            action = '3';       // ввод закончился — сбегаем
        }

        if (action == '1') {
            const int dmg = pistol(rng);
            impHealth -= dmg;
            std::cout << "You hit for " << dmg << ". ";
        } else if (action == '2') {
            if (medkits > 0) {
                --medkits;
                playerHealth += 25;
                if (playerHealth > 100) {
                    playerHealth = 100;
                }
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
            break;              // мёртвые не стреляют
        }
        if (percent(rng) <= 60) {   // попадает в 60% случаев
            const int dmg = fireball(rng);
            playerHealth -= dmg;
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
