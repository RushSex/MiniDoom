#include <iostream>

struct Game {
    int health = 100;
    bool dead = false;

    void hurt(int damage) {
        health -= damage;
        if (health == 0) {
            dead = true;
        }
    }
};

int main() {
    Game g;
    for (int damage : {30, 30, 30, 15}) {
        g.hurt(damage);
    }
    std::cout << "health " << g.health << ", dead " << std::boolalpha
              << g.dead << '\n';
}
