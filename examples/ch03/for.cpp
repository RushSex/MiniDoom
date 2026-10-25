#include <iostream>

int main() {
    int health = 65;
    int filled = health / 5;          // клетка — 5 единиц здоровья

    std::cout << "HEALTH [";
    for (int i = 0; i < 20; ++i) {    // 20 клеток в полоске
        if (i < filled) {
            std::cout << '#';
        } else {
            std::cout << '-';
        }
    }
    std::cout << "] " << health << "%\n";
}
