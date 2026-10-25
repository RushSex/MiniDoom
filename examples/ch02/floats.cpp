#include <format>
#include <iostream>

int main() {
    double a = 0.1 + 0.2;
    std::cout << std::format("{}\n", a == 0.3);
    std::cout << std::format("{:.17f}\n", a);
    std::cout << std::format("{:.17f}\n", 0.3);
}
