#include <iostream>

void hitCopy(int health, int damage) {  // получает копию
    health -= damage;
    std::cout << "  inside hitCopy: " << health << '\n';
}

void hitRef(int& health, int damage) {  // ссылка на оригинал
    health -= damage;
    std::cout << "  inside hitRef:  " << health << '\n';
}

int main() {
    int health = 100;
    hitCopy(health, 30);
    std::cout << "after hitCopy: " << health << '\n';
    hitRef(health, 30);
    std::cout << "after hitRef:  " << health << '\n';
}
