#include <iostream>

int main() {
    for (int tick = 1; tick <= 15; ++tick) {
        std::cout << tick << ": ";
        if (tick % 5 == 0 && tick % 3 == 0) {
            std::cout << "BOSS!";
        } else if (tick % 5 == 0) {
            std::cout << "imp spawns";
        } else if (tick % 3 == 0) {
            std::cout << "zombie spawns";
        } else {
            std::cout << "...";
        }
        std::cout << '\n';
    }
}
