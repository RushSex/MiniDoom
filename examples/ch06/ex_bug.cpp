#include <iostream>
#include <vector>

struct Monster {
    int health = 20;
};

int main() {
    std::vector<Monster> monsters(3);
    for (Monster m : monsters) {   // взрыв бочки задевает всех
        m.health -= 15;
    }
    for (const Monster& m : monsters) {
        std::cout << m.health << ' ';
    }
    std::cout << '\n';
}
