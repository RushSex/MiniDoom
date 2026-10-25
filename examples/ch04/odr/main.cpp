#include "rules.hpp"

#include <iostream>

int healthPercent(int health);

int main() {
    std::cout << healthPercent(50) << "% of " << maxHealth() << '\n';
}
