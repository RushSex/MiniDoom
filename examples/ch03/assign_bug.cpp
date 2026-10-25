#include <iostream>

int main() {
    int health = 40;
    if (health = 0) {        // хотели сравнить, а присвоили
        std::cout << "You are dead.\n";
    }
    std::cout << "health: " << health << '\n';
}
