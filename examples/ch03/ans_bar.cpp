#include <iostream>

int main() {
    int ammo = 37;
    std::cout << "AMMO ";
    for (int i = 0; i < ammo / 10; ++i) {
        std::cout << '|';
    }
    std::cout << ' ' << ammo << '\n';
}
