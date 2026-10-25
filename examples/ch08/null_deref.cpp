#include <iostream>

struct Monster {
    int health = 20;
};

Monster* findBoss() {
    return nullptr;          // босса на уровне нет
}

int main() {
    Monster* boss = findBoss();
    std::cout << "boss health: " << boss->health << '\n';  // упс
}
