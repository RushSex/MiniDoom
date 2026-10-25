#include <iostream>

int main() {
    int impHealth = 60;
    int shots = 0;

    while (impHealth > 0) {   // пока имп жив — стреляем
        impHealth -= 15;
        ++shots;
        std::cout << "BANG! Imp health: " << impHealth << '\n';
    }
    std::cout << "Imp is dead after " << shots << " shots.\n";
}
