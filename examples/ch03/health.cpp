#include <iostream>

int main() {
    int health = 0;
    std::cout << "Enter health: ";
    std::cin >> health;

    if (health <= 0) {
        std::cout << "You are dead.\n";
    } else if (health < 25) {
        std::cout << "LOW HEALTH! Find a medkit.\n";
    } else {
        std::cout << "You are fine. Keep fighting.\n";
    }
}
