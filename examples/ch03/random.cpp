#include <iostream>
#include <random>

int main() {
    std::mt19937 rng(42);                              // «зерно» 42
    std::uniform_int_distribution<int> damage(5, 15);  // от 5 до 15

    for (int shot = 1; shot <= 8; ++shot) {
        std::cout << "shot " << shot << ": " << damage(rng) << '\n';
    }
}
