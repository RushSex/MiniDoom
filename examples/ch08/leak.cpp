#include <iostream>

struct Monster {
    int health = 20;
};

int main() {
    Monster* imp = new Monster{60};   // взяли память в куче
    std::cout << "imp health: " << imp->health << '\n';
    // ...а вернуть забыли: delete imp; нигде нет
}
