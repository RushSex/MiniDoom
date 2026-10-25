#include <iostream>

int main() {
    int health = 100;
    double speed = 3.4;
    char key = 'W';
    bool alive = true;

    std::cout << "health: " << sizeof(health) << " B at "
              << &health << '\n';
    std::cout << "speed:  " << sizeof(speed) << " B at "
              << &speed << '\n';
    std::cout << "key:    " << sizeof(key) << " B at "
              << static_cast<void*>(&key) << '\n';
    std::cout << "alive:  " << sizeof(alive) << " B at "
              << &alive << '\n';
}
