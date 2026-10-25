#include <iostream>
#include <random>

int main() {
    std::mt19937 rng(7);
    std::uniform_int_distribution<int> digits(100, 999);
    const int code = digits(rng);

    int tries = 0;
    int guess = 0;
    std::cout << "Door code is a number 100..999.\n";
    while (guess != code) {
        std::cout << "Code: ";
        if (!(std::cin >> guess)) {
            std::cout << "\nGave up.\n";
            return 0;
        }
        ++tries;
        if (guess < code) {
            std::cout << "Too low.\n";
        } else if (guess > code) {
            std::cout << "Too high.\n";
        }
    }
    std::cout << "Door opens after " << tries << " tries.\n";
}
